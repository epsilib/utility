#ifndef EPSILIB_UTILITY_TICTOC_HPP_INCLUDED
#define EPSILIB_UTILITY_TICTOC_HPP_INCLUDED

#include <chrono>
#include <vector>
#include <string>
#include <sstream>

#include <epsilib/utility/container_helpers.hpp>

namespace epsilib { namespace utility {


class TicTocTimer {
 public:
  inline void tic(){
    m_tic_time = std::chrono::high_resolution_clock::now();
  }
  inline double toc(){
    m_toc_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = m_toc_time - m_tic_time;
    return elapsed.count();  // return elapsed time in seconds
  }
private:
  std::chrono::high_resolution_clock::time_point m_tic_time;
  std::chrono::high_resolution_clock::time_point m_toc_time;
};


class TicTocLogger {
public:
  TicTocLogger(std::string comment = "") : m_comment(comment) {}
  inline void tic() {
    m_tic_toc_timer.tic();
  }
  inline void toc() {
    m_log.push_back(m_tic_toc_timer.toc());
  };
  double elapsed() const {return m_log.back();}
  double mean_elapsed()  const {return epsilib::utility::mean(m_log);} 
  double max_elapsed()   const {return epsilib::utility::max(m_log);} 
  double min_elapsed()   const {return epsilib::utility::min(m_log);} 
  double stdev_elapsed() const {return epsilib::utility::stdev(m_log);} 
  const std::vector<double> &log() const {return m_log;}
  void clear_log() {m_log.clear();}
  std::string stat_string() const {
    std::ostringstream ss;
    ss << m_comment
       << "  mean: "     << epsilib::utility::to_eng_string(mean_elapsed())
       << "     max: "   << epsilib::utility::to_eng_string(max_elapsed())
       << "     min: "   << epsilib::utility::to_eng_string(min_elapsed())
       << "     stdev: " << epsilib::utility::to_eng_string(stdev_elapsed());
    return ss.str();
  }

private:
  const std::string m_comment;
  TicTocTimer m_tic_toc_timer;
  std::vector<double> m_log;
};


} }  // namespace epsilib::utility

#endif  // EPSILIB_UTILITY_TICTOC_HPP_INCLUDED
