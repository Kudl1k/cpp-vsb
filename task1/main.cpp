#include "tasks.h"
#include <iostream>

int main() {
  std::cout << math::fibonacci(15) << std::endl;
  std::cout << strutils::camel_to_snake("foo") << std::endl;
  u_int32_t result = 0;
  if (strutils::parse_uint("9el", result)) {
    std::cout << result << std::endl;
  }
  size_t result2 = ' ';
  strutils::validate_utf8({0b00001100}, result2);
}
