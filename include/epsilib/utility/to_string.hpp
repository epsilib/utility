#ifndef EPSILIB_UTILITY_TO_STRING_HPP_INCLUDED
#define EPSILIB_UTILITY_TO_STRING_HPP_INCLUDED

#include <cmath>
#include <string>
#include <iomanip>

namespace epsilib { namespace utility {

//
//  to_eng_string function
//
template <typename T>
std::string to_eng_string(T value, int precision = 3, bool fixed_width = true) {

  // todo: clean up this function and write some tests for it
  // todo: handle the case when value is zero and precision != 3

  using std::abs;
  using std::log10;
  using std::floor;

  // modify the input parameters
  double val = static_cast<double>(value);

  if (precision < 3) {
    precision = 3;
  }

  std::ostringstream ss;
  ss << std::fixed;

  int exponent;
  int whole_digits;

  if (abs(val) < 1e-200) {
    val = 0.0;
    exponent = 0;
    whole_digits = 0;
  } else {
    exponent = static_cast<int>(floor(log10(abs(val)) / 3.0)) * 3;
    val /= std::pow(10, exponent);
    whole_digits = static_cast<int>(floor(log10(abs(val))));
  }

  ss << std::setprecision(precision - whole_digits);

  if (fixed_width) {
    ss.width(precision + 3);
    if (exponent == 0) {
      ss << val << "   ";
    } else {
      ss << val << 'e';
      ss.width(2);
      ss << std::left << exponent;
    }
  } else {
    if (exponent == 0) {
      ss << val;
    } else {
      ss << val << 'e' << exponent;
    }
  }

  return ss.str();
}



} }  // namespace epsilib::utility

#endif  // EPSILIB_UTILITY_TO_STRING_HPP_INCLUDED
