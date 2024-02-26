#pragma once

#include <string>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>
#include <optional>



using CodePoint = uint32_t;

class UTF8String {
private:
    size_t points = 0;
    size_t bytes = 0;
    size_t reserved = 0;

    void control_grow(size_t newSize);
    uint8_t *buffer = nullptr;
public:
    

    UTF8String() = default;
    UTF8String(const char* ptr);
    UTF8String(const std::vector<CodePoint> ptr);
    UTF8String(const UTF8String &ptr);

    UTF8String& operator=(const UTF8String &ptr);

    int get_number_of_bytes(CodePoint ptr);

    int get_byte_count() const;
    int get_point_count() const;
    int get_reserved_count() const;

    std::optional<uint8_t> operator[](const size_t idx) const;

    std::optional<uint8_t> nth_code_point(const size_t idx) const;


    void append(const char c);
    void append(const CodePoint cp);


    friend UTF8String operator+(const UTF8String &lhs,const UTF8String &rhs);

    UTF8String& operator+=(const UTF8String& str);

    friend bool operator==(const UTF8String &lhs,const UTF8String &rhs);



    operator std::string() const {
        std::string result;
        for (size_t i = 0; i < points; i++)
        {
            result.push_back(static_cast<char>(buffer[i]));
        }
        return result;
    }

    ~UTF8String();
};



