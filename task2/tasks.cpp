#include "tasks.h"



UTF8String::~UTF8String(){
    if (buffer != nullptr)
    {
        delete[] buffer;
    }
}


UTF8String::UTF8String(const char* ptr){
    points = strlen(ptr);
    bytes = points;
    buffer = new CodePoint[points];
    reserved = points;
    for (size_t i = 0; i < points; i++)
    {
        buffer[i] = static_cast<CodePoint>(ptr[i]);
    }
}

UTF8String::UTF8String(const std::vector<CodePoint> ptr){
    points = ptr.size();
    buffer = new CodePoint[points];
    reserved = points;
    for (size_t i = 0; i < points; i++)
    {
        buffer[i] = ptr[i];
        bytes += get_number_of_bytes(ptr[i]);
    }
}

UTF8String::UTF8String(const UTF8String &ptr){
    points = ptr.points;
    bytes = ptr.bytes;
    buffer = new CodePoint[points];
    reserved = points;
    for (size_t i = 0; i < points; i++)
    {
        buffer[i] = ptr.buffer[i];
    }
}



int UTF8String::get_number_of_bytes(CodePoint ptr){
    if (ptr <= 0x007F)
    {
        return 1;
    } else if (ptr <= 0x07FF)
    {
        return 2;
    } else if (ptr <= 0xFFFF)
    {
        return 3;
    } else
    {
        return 4;
    }
}


int UTF8String::get_point_count() const {
    return points;
}

int UTF8String::get_byte_count() const {
    return bytes;
}

int UTF8String::get_reserved_count() const {
    return reserved;
}


std::optional<CodePoint> UTF8String::operator[](const size_t idx){
    if (idx >= points)
    {
        return std::nullopt;   
    } else {
        return buffer[idx];
    }
}


std::optional<CodePoint> UTF8String::nth_code_point(const size_t idx){
    if (idx >= points)
    {
        return std::nullopt;   
    } else {
        return buffer[idx];
    }
}


void UTF8String::control_grow(size_t newSize){
    size_t size = std::max(newSize, points * 2);
    CodePoint* newBuffer = new CodePoint[size];
    std::memcpy(newBuffer, buffer, points * sizeof(CodePoint));
    delete[] buffer;
    buffer = newBuffer;
    reserved = size;
}


void UTF8String::append(const char c){
    if (points + 1 > reserved) {
        control_grow(points + 1);
    }
    buffer[points] = static_cast<CodePoint>(c);
    points +=  1;
    bytes += get_number_of_bytes(static_cast<CodePoint>(c));
}

void UTF8String::append(const CodePoint cp){
    if (points + 1 > reserved) {
        control_grow(points + 1);
    }
    buffer[points] = cp;
    points +=  1;
    bytes += get_number_of_bytes(cp);
}

UTF8String operator+(const UTF8String &lhs,const UTF8String &rhs){
    size_t newSize = lhs.points + rhs.points;
    UTF8String result;
    result.points = newSize;
    result.reserved = result.points + 1;
    result.bytes = lhs.bytes + rhs.bytes;
    result.buffer = new CodePoint[result.reserved];
    std::copy(lhs.buffer, lhs.buffer + lhs.points, result.buffer);
    std::copy(rhs.buffer, rhs.buffer + rhs.reserved, result.buffer + lhs.points);
    return result;
}