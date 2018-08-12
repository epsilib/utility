#ifndef EPSILIB_TICTOC_HPP_INCLUDED
#define EPSILIB_TICTOC_HPP_INCLUDED

#include <memory>
#include <vector>
#include <string>
#include <sstream>

#include <epsilib/utility/container_helpers.hpp>

namespace epsilib { namespace utility {



class TicTocTimer {
 public:
  virtual void tic() = 0;    // starts the timer
  virtual double toc() = 0;  // returns timer value in seconds
  double get_elapsed() const {return log_.back();}
  double get_mean_elapsed()  const {return epsilib::utility::mean(log_);} 
  double get_max_elapsed()   const {return epsilib::utility::max(log_);} 
  double get_min_elapsed()   const {return epsilib::utility::min(log_);} 
  double get_stdev_elapsed() const {return epsilib::utility::stdev(log_);} 
  void clear_log() {log_.clear();}
  std::string comment;
  std::string stat_string() const;

 protected:
  double log_elapsed(double value) {log_.push_back(value); return value;}

 private:
  std::vector<double> log_;
};

class SystemTicTocTimer : public TicTocTimer {
 public:
  virtual void tic();
  virtual double toc();
  SystemTicTocTimer();
  ~SystemTicTocTimer();
  SystemTicTocTimer(SystemTicTocTimer&& rhs);
  SystemTicTocTimer& operator=(SystemTicTocTimer&& rhs);
  SystemTicTocTimer(SystemTicTocTimer& rhs);
  SystemTicTocTimer& operator=(SystemTicTocTimer& rhs);

 protected:
  class Impl;  // PIMPL Idiom
  std::unique_ptr<Impl> pImpl;
};

class ProcessTicTocTimer : public TicTocTimer {
 public:
  virtual void tic();
  virtual double toc();
  ProcessTicTocTimer();
  ~ProcessTicTocTimer();
  ProcessTicTocTimer(ProcessTicTocTimer&& rhs);
  ProcessTicTocTimer& operator=(ProcessTicTocTimer&& rhs);
  ProcessTicTocTimer(ProcessTicTocTimer& rhs);
  ProcessTicTocTimer& operator=(ProcessTicTocTimer& rhs);

 protected:
  class Impl;  // PIMPL Idiom
  std::unique_ptr<Impl> pImpl;
};

class ThreadTicTocTimer : public TicTocTimer {
 public:
  virtual void tic();
  virtual double toc();
  ThreadTicTocTimer();
  ~ThreadTicTocTimer();
  ThreadTicTocTimer(ThreadTicTocTimer&& rhs);
  ThreadTicTocTimer& operator=(ThreadTicTocTimer&& rhs);
  ThreadTicTocTimer(ThreadTicTocTimer& rhs);
  ThreadTicTocTimer& operator=(ThreadTicTocTimer& rhs);

 protected:
  class Impl;  // PIMPL Idiom
  std::unique_ptr<Impl> pImpl;
};


} }  // namespace epsilib::utility

#endif  // EPSILIB_TICTOC_HPP_INCLUDED
