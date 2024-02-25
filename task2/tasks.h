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
    size_t points = 0;
    size_t bytes = 0;
    size_t reserved = 0;
public:
    CodePoint *buffer = nullptr;

    UTF8String() = default;
    UTF8String(const char* ptr);
    UTF8String(std::vector<CodePoint>);

    int get_number_of_bytes(CodePoint *ptr);

    int get_byte_count();
    int get_point_count();


    ~UTF8String();
};
