#include "tasks.h"
#include <iostream>

int main() {
  std::cout << math::fibonacci(15) << std::endl;
  std::cout << strutils::camel_to_snake("foo") << std::endl;
  u_int32_t result = 0;
  if (strutils::parse_uint("4294967295", result)) {
    std::cout << result << std::endl;
  }
  size_t result2 = ' ';
  std::vector<uint8_t> input{
                0b11111111
        };
  if (strutils::validate_utf8(input, result2))
  {
    std::cout << result2 << std::endl;
  }
  
}
