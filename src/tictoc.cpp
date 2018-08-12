
#include <cmath>
#include <limits>

#include <epsilib/utility/to_string.hpp>
#include <epsilib/utility/tictoc.hpp>


#ifdef __linux__
  // Includes for Linux
  #include <time.h>
#elif _WIN32
  // Includes for Windows
  #include <windows.h>
#endif


namespace epsilib { namespace utility {

//
// class TicTocTimer
//
std::string TicTocTimer::stat_string() const {
  std::ostringstream ss;
  if (!comment.empty()) {
    std::string formatted_comment = comment;
    formatted_comment.resize(20, ' ');
    ss << formatted_comment;
  }
  ss << "  mean: " << to_eng_string(get_mean_elapsed())
     << "     max: " << to_eng_string(get_max_elapsed())
     << "     min: " << to_eng_string(get_min_elapsed())
     << "     stdev: " << to_eng_string(get_stdev_elapsed());
  return ss.str();
}

//
// class SystemTicTocTimer
//
#ifdef __linux__
  class SystemTicTocTimer::Impl {
   private:
    struct timespec tic_time_;
    struct timespec toc_time_;

   public:
    void tic() {
      clock_gettime(CLOCK_MONOTONIC, &tic_time_);
    }

    double toc() {
      clock_gettime(CLOCK_MONOTONIC, &toc_time_);

      return (toc_time_.tv_sec  - tic_time_.tv_sec)                   // delta seconds
           + (toc_time_.tv_nsec - tic_time_.tv_nsec) / 1000000000.0;  // delta nanoseconds
    }

  };

#elif _WIN32
  class SystemTicTocTimer::Impl
  {
   public:
    Impl()
      : start_tick_ok_(FALSE), end_tick_ok_(FALSE), frequency_ok_(FALSE) {
    }

    void tic() {
      start_tick_ok_ = QueryPerformanceCounter(&start_tick_);
    }

    double toc() {
      end_tick_ok_ = QueryPerformanceCounter(&end_tick_);
      frequency_ok_ = QueryPerformanceFrequency(&ticks_per_second_);

      if (!(start_tick_ok_ && end_tick_ok_ && frequency_ok_)) {
        return std::numeric_limits<double>::quiet_NaN();
      }

      return static_cast<double>(end_tick_.QuadPart - start_tick_.QuadPart)
                          / ticks_per_second_.QuadPart;
    }

   private:
    BOOL start_tick_ok_, end_tick_ok_, frequency_ok_;
    LARGE_INTEGER start_tick_, end_tick_, ticks_per_second_;
  };

#else
  class SystemTicTocTimer::Impl
  {
   public:
    void tic() {}
    double toc() {
      return std::numeric_limits<double>::quiet_NaN();
    }
  };

#endif

void SystemTicTocTimer::tic() {
  pImpl->tic();
}

double SystemTicTocTimer::toc() {
  return log_elapsed(pImpl->toc());
}

SystemTicTocTimer::SystemTicTocTimer()      // ctor
: pImpl(std::make_unique<Impl>()) {}

SystemTicTocTimer::~SystemTicTocTimer() = default;  // destructor

SystemTicTocTimer::SystemTicTocTimer(SystemTicTocTimer&& rhs) = default;   // move ctor

SystemTicTocTimer& SystemTicTocTimer::operator=(SystemTicTocTimer&& rhs) = default;  // move operator=

SystemTicTocTimer::SystemTicTocTimer(SystemTicTocTimer& rhs)    // copy ctor
: pImpl(std::make_unique<Impl>(*rhs.pImpl)) {}

SystemTicTocTimer& SystemTicTocTimer::operator=(SystemTicTocTimer& rhs) { // copy operator=
  *pImpl = *rhs.pImpl;
  return *this;
}


//
// class ProcessTicTocTimer
//
#ifdef __linux__
  class ProcessTicTocTimer::Impl
  {
   public:
    void tic() {
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tic_time_);
    }

    double toc() {
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &toc_time_);

      return (toc_time_.tv_sec  - tic_time_.tv_sec)                   // delta seconds
           + (toc_time_.tv_nsec - tic_time_.tv_nsec) / 1000000000.0;  // delta nanoseconds
    }

   private:
    struct timespec tic_time_;
    struct timespec toc_time_;
  };


#elif _WIN32
  // Windows implementation of ProcessTicTocTimer.
  // From experimentation, this timer seems to have a resolution of
  // 15.652 milliseconds.

  class ProcessTicTocTimer::Impl
  {
   public:
    Impl()
      : start_time_ok_(FALSE), end_time_ok_(FALSE) {
    }

    void tic() {
      process_ = GetCurrentProcess();
      start_time_ok_ = GetProcessTimes(process_,
                                       &process_create_time_,
                                       &process_exit_time_,
                                       &start_kernel_time_,
                                       &start_user_time_);
    }

    double toc() {
      end_time_ok_ = GetProcessTimes(process_,
                                     &process_create_time_,
                                     &process_exit_time_,
                                     &end_kernel_time_,
                                     &end_user_time_);

      if (!(start_time_ok_ && end_time_ok_)) {
        return std::numeric_limits<double>::quiet_NaN();
      }

      double elapsed_kernel_seconds =
        calc_elapsed_seconds_(start_kernel_time_, end_kernel_time_);

      double elapsed_user_seconds =
        calc_elapsed_seconds_(start_user_time_, end_user_time_);

      return elapsed_kernel_seconds + elapsed_user_seconds;
    }

   private:
    HANDLE process_;
    BOOL start_time_ok_, end_time_ok_;
    FILETIME process_create_time_;
    FILETIME process_exit_time_;
    FILETIME start_kernel_time_;
    FILETIME start_user_time_;
    FILETIME end_kernel_time_;
    FILETIME end_user_time_;

    double calc_elapsed_seconds_(FILETIME& start_filetime,
                                 FILETIME& end_filetime) {
      ULARGE_INTEGER start_ticks;
      ULARGE_INTEGER end_ticks;

      start_ticks.u.LowPart = start_filetime.dwLowDateTime;
      start_ticks.u.HighPart = start_filetime.dwHighDateTime;

      end_ticks.u.LowPart = end_filetime.dwLowDateTime;
      end_ticks.u.HighPart = end_filetime.dwHighDateTime;

      // There are 10,000,000 ticks per second
      return (end_ticks.QuadPart - start_ticks.QuadPart) / 10000000.0;
    }

  };

#else
  class ProcessTicTocTimer::Impl
  {
   public:
    void tic() {}

    double toc() {
      return std::numeric_limits<double>::quiet_NaN();
    }
  };

#endif

void ProcessTicTocTimer::tic() {
  pImpl->tic();
}

double ProcessTicTocTimer::toc() {
  return log_elapsed(pImpl->toc());
}

ProcessTicTocTimer::ProcessTicTocTimer()      // ctor
  : pImpl(std::make_unique<Impl>()) {
}

ProcessTicTocTimer::~ProcessTicTocTimer() = default;  // destructor

ProcessTicTocTimer::ProcessTicTocTimer(ProcessTicTocTimer&& rhs) = default;   // move ctor

ProcessTicTocTimer& ProcessTicTocTimer::operator=(ProcessTicTocTimer&& rhs) = default;  // move operator=

ProcessTicTocTimer::ProcessTicTocTimer(ProcessTicTocTimer& rhs)    // copy ctor
  : pImpl(std::make_unique<Impl>(*rhs.pImpl)) {
}

ProcessTicTocTimer& ProcessTicTocTimer::operator=(ProcessTicTocTimer& rhs) { // copy operator=
  *pImpl = *rhs.pImpl;
  return *this;
}


//
// class ThreadTicTocTimer
//
#ifdef __linux__
  class ThreadTicTocTimer::Impl
  {
   public:
    void tic() {
      clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tic_time_);
    }

    double toc() {
      clock_gettime(CLOCK_THREAD_CPUTIME_ID, &toc_time_);

      return (toc_time_.tv_sec  - tic_time_.tv_sec)                   // delta seconds
           + (toc_time_.tv_nsec - tic_time_.tv_nsec) / 1000000000.0;  // delta nanoseconds
    }

   private:
    struct timespec tic_time_;
    struct timespec toc_time_;
  };


#elif _WIN32
  class ThreadTicTocTimer::Impl
  {
   public:
    void tic() {}
    double toc() {
      return std::numeric_limits<double>::quiet_NaN();
    }
  };

#else
  class ThreadTicTocTimer::Impl
  {
   public:
    void tic() {}
    double toc() {
        return std::numeric_limits<double>::quiet_NaN();
    }
  };

#endif

void ThreadTicTocTimer::tic() {
  pImpl->tic();
}

double ThreadTicTocTimer::toc() {
  return log_elapsed(pImpl->toc());
}

ThreadTicTocTimer::ThreadTicTocTimer()      // ctor
  : pImpl(std::make_unique<Impl>()) {
}

ThreadTicTocTimer::~ThreadTicTocTimer() = default;  // destructor

ThreadTicTocTimer::ThreadTicTocTimer(ThreadTicTocTimer&& rhs) = default;   // move ctor

ThreadTicTocTimer& ThreadTicTocTimer::operator=(ThreadTicTocTimer&& rhs) = default;  // move operator=

ThreadTicTocTimer::ThreadTicTocTimer(ThreadTicTocTimer& rhs)    // copy ctor
  : pImpl(std::make_unique<Impl>(*rhs.pImpl)) {
}

ThreadTicTocTimer& ThreadTicTocTimer::operator=(ThreadTicTocTimer& rhs) {  // copy operator=
  *pImpl = *rhs.pImpl;
  return *this;
}


} }  // namespace epsilib::utility
