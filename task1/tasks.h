#pragma once

#include <cstddef>
#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>

namespace math {
int fibonacci(int n);
}

namespace strutils {
std::string camel_to_snake(std::string s);
bool parse_uint(const std::string &s, u_int32_t &result);
bool validate_utf8(const std::vector<uint8_t> &bytes, size_t &result);
} // namespace strutils
