#pragma once

#include <string>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>



using CodePoint = uint32_t;

class UTF8String {
private:
    CodePoint* buffer;
    size_t size = 0;
    size_t reserved = 0;
public:
    UTF8String() = default;
    UTF8String(const char* ptr);


    friend std::ostream& operator<<(std::ostream& os, const UTF8String& str);


    ~UTF8String();
};
