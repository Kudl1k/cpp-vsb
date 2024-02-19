#include "tasks.h"
#include <cctype>
#include <cmath>
#include <cstdint>
#include <string>

int math::fibonacci(int n) {
  if (n <= 1) {
    return n;
  } else {
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
}

std::string strutils::camel_to_snake(std::string s) {
  std::string result = "";

  for (char c : s) {
    if (isupper(c)) {
      result += "_";
      result += tolower(c);
    } else {
      result += c;
    }
  }
  return result;
}

bool strutils::parse_uint(const std::string &s, u_int32_t &result) {
  u_int32_t rank = s.length();
  for (char c : s) {
    if (c >= '0' && c <= '9') {
      rank--;

      result += (c - '0') * pow(10, rank);
    } else {
      return false;
    }
  }
  if (result > UINT32_MAX) {
    return false;
  }
  return true;
}

bool strutils::validate_utf8(const std::vector<uint8_t> &bytes,
                             size_t &result) {
  return true;
}
