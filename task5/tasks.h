#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>
#include <optional>
#include <vector>

/// UTF-8 string (reuse your previous implementation and modify it)

using CodePoint = uint32_t;





using CodePoint = uint32_t;

class UTF8String {
public:
    

    UTF8String() = default;
    UTF8String(const std::string& s);
    UTF8String(const std::vector<CodePoint>&ptr);
    UTF8String(const UTF8String &ptr);
    UTF8String(UTF8String&& other);

    UTF8String& operator=(const UTF8String &ptr);



    int get_number_of_bytes(CodePoint ptr);

    int get_byte_count() const;
    int get_point_count() const;
    int get_reserved_count() const;

    std::optional<uint8_t> operator[](const size_t idx) const;

    std::optional<CodePoint> nth_code_point(const size_t idx) const;


    void append(const char c);
    void append(const CodePoint cp);


    friend UTF8String operator+(const UTF8String &lhs,const UTF8String &rhs);

    UTF8String& operator+=(const UTF8String& str);
    UTF8String& operator=(UTF8String&& other);

    friend bool operator==(const UTF8String &lhs,const UTF8String &rhs);
    friend bool operator!=(const UTF8String &lhs,const UTF8String &rhs);



    operator std::string() const {
        std::string result;
        for (size_t i = 0; i < points; i++)
        {
            result.push_back(static_cast<char>(buffer.at(i)));
        }
        return result;
    }

    ~UTF8String() = default;
private:
    size_t points = 0;
    size_t bytes = 0;

    void control_grow(size_t newSize);
    std::vector<uint8_t> buffer;
};




/// Binary tree
// Big data that we cannot afford to copy
struct BigData {
    explicit BigData(int value): value(value) {}

    BigData(const BigData&) = delete;
    BigData& operator=(const BigData&) = delete;

    int value;
};

class Tree;
