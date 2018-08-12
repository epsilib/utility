#ifndef EPSILIB_UTILITY_CONTAINER_HELPERS_HPP_INCLUDED
#define EPSILIB_UTILITY_CONTAINER_HELPERS_HPP_INCLUDED

#include <cmath>
#include <numeric>
#include <algorithm>

namespace epsilib { namespace utility {


//
//  sum()
//  Sum the elements of an STL container.
//
template <typename Container>
typename Container::value_type sum(const Container& input) {
  auto result = *(input.cbegin());
  result = std::accumulate(input.cbegin() + 1, input.cend(), result);
  return result;
}

//
//  dsum()
//  Cast the elements of an STL container to double and calculate the sum.
//  
template <typename Container>
double dsum(const Container& input) {
  return std::accumulate(input.cbegin(), input.cend(), 0.0);
}

//
//  mean()
//  Calclate the mean of all elements in an STL container.
//
template <typename Container>
double mean(const Container& input) {
  return dsum(input) / input.size();
}

//
//  max()
//  Return the largest element of an STL container.
//
template <typename Container>
typename Container::value_type max(const Container& input) {
  return *std::max_element(input.cbegin(), input.cend());
}

//
//  min()
//  Return the smallest element of an STL container.
//
template <typename Container>
typename Container::value_type min(const Container& input) {
  return *std::min_element(input.cbegin(), input.cend());
}

//
//  stdef()
//  Calculate the standard deviation of the elements in an STL container.
//
template <typename Container>
double stdev(const Container& input) {
  // todo: handle case when input contains one element
  //       or when there is no variation in the data
  double mean_val = mean(input);
  double result = std::accumulate(
    input.cbegin(),
    input.cend(),
    0.0,
    [&mean_val](const double& tally, const double& element_val) {
      return tally + std::pow((element_val - mean_val), 2);
    });
  return std::sqrt(result / (input.size() - 1));
}



} }  // namespace epsilib::utility

#endif  // EPSILIB_UTILITY_CONTAINER_HELPERS_HPP_INCLUDED
