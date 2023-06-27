#pragma once
#include "complex.hpp"
#include <cmath>

template <typename T>
T Abs(const T& number) {
  return (number > 0)? number : -number;
}
