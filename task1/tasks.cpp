#include "tasks.h"
#include <cctype>
#include <cmath>
#include <cstdint>
#include <string>


/*
iterative instead of recursive, because of speed;
*/
int math::fibonacci(int n) {
  if (n == 0 || n == 1)
  {
    return n;
  }
  int curentNumber = 0;
  int nextNumber = 1;
  int result = 0;
  for (int i = 0; i < n-1; i++)
  {
    result = curentNumber + nextNumber;
    curentNumber = nextNumber;
    nextNumber = result;
  }
  return result;
}

/*
rewrite
*/
std::string strutils::camel_to_snake(std::string s) {
  std::string result = "";

  for (char c : s) {
    if (isupper(c)) {
      if (result.empty())
      {
        result+= tolower(c);
      } else {
        result += "_";
        result += tolower(c);
      }
    } else {
      result += c;
    }
  }
  return result;
}


/*
:)
*/
bool strutils::parse_uint(const std::string &s, u_int32_t &result) {
  if (s.empty())
  {
    return false;
  }
  u_int32_t temp = 0;
  for (char c : s) {
    if (c >= '0' && c <= '9') {
      u_int32_t digit = (c - '0');
      if (temp > UINT32_MAX / 10)
      {
        return false;
      }
      temp *= 10;
      if (temp > UINT32_MAX - digit)
      {
        return false;
      }
      temp += digit;
      
    } else {
      return false;
    }
  }
  result = temp;
  return true;
}

bool strutils::validate_utf8(const std::vector<uint8_t> &bytes, size_t &result) {
  size_t temp = 0;

  if (bytes.size() == 0)
  {
    result = 0;
    return true;
  }
  
  for (size_t i = 0; i < bytes.size();)
  {
    if (i+3 < bytes.size() && (bytes[i] & 0b11110000) == 0b11110000 && (bytes[i+1] & 0b10000000) == 0b10000000 && (bytes[i+2] & 0b10000000) == 0b10000000 && (bytes[i+3] & 0b10000000) == 0b10000000)
    {
      temp += 1;
      i += 4;
      continue;
    }
    if (i+2 < bytes.size() && (bytes[i] & 0b11100000) == 0b11100000 && (bytes[i+1] & 0b10000000) == 0b10000000 && (bytes[i+2] & 0b10000000) == 0b10000000)
    {
      temp += 1;
      i +=3;
      continue;
    }
    if (i+1 < bytes.size() && (bytes[i] & 0b11000000) == 0b11000000 && (bytes[i+1] & 0b10000000) == 0b10000000)
    {
      temp += 1;
      i += 2;
      continue;
    }
    if ((bytes[i] & 0b10000000) == 0)
    {
      i+=1;
      temp += 1;
      continue;
    }
    return false;
  }
  result = temp;  
  return true;
}
