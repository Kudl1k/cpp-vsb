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
    buffer = new uint8_t[points];
    reserved = points;
    for (size_t i = 0; i < points; i++)
    {
        buffer[i] = static_cast<uint8_t>(ptr[i]);
    }
}

UTF8String::UTF8String(const std::vector<CodePoint> ptr){
    points = ptr.size();
    for (size_t i = 0; i < points; i++)
    {
        bytes += get_number_of_bytes(ptr[i]);
    }
    buffer = new uint8_t[bytes];
    reserved = points;
    size_t idx = 0;

    for (size_t i = 0; i < points;i++)
    {
        if (ptr[i] <=   0x7F) {
            buffer[idx++] = static_cast<uint8_t>(ptr[i]);
        } else if (ptr[i] <=   0x7FF) {
            buffer[idx++] = static_cast<uint8_t>(0xC0 | (ptr[i] >>   6));
            buffer[idx++] = static_cast<uint8_t>(0x80 | (ptr[i] &   0x3F));
        } else if (ptr[i] <=   0xFFFF) {
            buffer[idx++] = static_cast<uint8_t>(0xE0 | (ptr[i] >>   12));
            buffer[idx++] = static_cast<uint8_t>(0x80 | ((ptr[i] >>   6) &   0x3F));
            buffer[idx++] = static_cast<uint8_t>(0x80 | (ptr[i] &   0x3F));
        } else if (ptr[i] <=   0x10FFFF) {
            buffer[idx++] = static_cast<uint8_t>(0xF0 | (ptr[i] >>   18));
            buffer[idx++] = static_cast<uint8_t>(0x80 | ((ptr[i] >>   12) &   0x3F));
            buffer[idx++] = static_cast<uint8_t>(0x80 | ((ptr[i] >>   6) &   0x3F));
            buffer[idx++] = static_cast<uint8_t>(0x80 | (ptr[i] &   0x3F));
        }
    }
}




UTF8String::UTF8String(const UTF8String &ptr){
    points = ptr.points;
    bytes = ptr.bytes;
    reserved = points;
    buffer = new uint8_t[reserved];
    std::copy(ptr.buffer, ptr.buffer + reserved, buffer);
}

UTF8String& UTF8String::operator=(const UTF8String &ptr){
    if (buffer != nullptr)
    {
        delete[] buffer;
    }
    points = ptr.points;
    reserved = ptr.reserved;
    bytes = ptr.bytes;
    buffer = new uint8_t[reserved];
    std::copy(ptr.buffer, ptr.buffer + reserved, buffer);

    return *this;
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


std::optional<uint8_t> UTF8String::operator[](const size_t idx) const{
    if (idx >= bytes)
    {
        return std::nullopt;   
    } else {
        return buffer[idx];
    }
}


std::optional<CodePoint> UTF8String::nth_code_point(const size_t idx) const {
    if (idx >= points) {
        return std::nullopt; // idx is out of bounds
    }

    size_t currentCodePoint =  0;
    size_t currentByte =  0;
    while (currentCodePoint < idx) {
        // Skip continuation bytes
        while ((buffer[currentByte] &  0xC0) ==  0x80) {
            currentByte++;
        }
        currentCodePoint++;
    }

    // At this point, currentByte points to the start of the idx-th code point
    // Decode the code point
    CodePoint codePoint =  0;
    size_t bytesInSequence =  0;
    if ((buffer[currentByte] &  0x80) ==  0) { //  1-byte sequence
        codePoint = buffer[currentByte];
        bytesInSequence =  1;
    } else if ((buffer[currentByte] &  0xE0) ==  0xC0) { //  2-byte sequence
        codePoint = buffer[currentByte] &  0x1F;
        bytesInSequence =  2;
    } else if ((buffer[currentByte] &  0xF0) ==  0xE0) { //  3-byte sequence
        codePoint = buffer[currentByte] &  0x0F;
        bytesInSequence =  3;
    } else if ((buffer[currentByte] &  0xF8) ==  0xF0) { //  4-byte sequence
        codePoint = buffer[currentByte] &  0x07;
        bytesInSequence =  4;
    }

    // Decode the remaining bytes
    for (size_t i =  1; i < bytesInSequence; ++i) {
        codePoint <<=  6; // Shift left to make room for the next  6 bits
        codePoint |= (buffer[currentByte + i] &  0x3F); // Add the next  6 bits
    }

    return codePoint;
}




void UTF8String::control_grow(size_t newSize){
    size_t size = std::max(newSize, bytes * 2);
    uint8_t* newBuffer = new uint8_t[size];
    std::memcpy(newBuffer, buffer, bytes * sizeof(uint8_t));
    delete[] buffer;
    buffer = newBuffer;
    reserved = size;
}


void UTF8String::append(const char c){
    if (bytes + 1 > reserved) {
        control_grow(bytes + 1);
    }
    buffer[bytes] = static_cast<CodePoint>(c);
    points +=  1;
    bytes += 1;
}

void UTF8String::append(const CodePoint cp){
    size_t cpBytes = get_number_of_bytes(cp);
    if (bytes + cpBytes > reserved) {
        control_grow(bytes + cpBytes);
    }
    size_t idx = bytes;
    if (cp <= 0x7F) {
        buffer[idx++] = static_cast<uint8_t>(cp);
    } else if (cp <= 0x7FF) {
        buffer[idx++] = static_cast<uint8_t>(0xC0 | (cp >> 6));
        buffer[idx++] = static_cast<uint8_t>(0x80 | (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        buffer[idx++] = static_cast<uint8_t>(0xE0 | (cp >> 12));
        buffer[idx++] = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3F));
        buffer[idx++] = static_cast<uint8_t>(0x80 | (cp & 0x3F));
    } else if (cp <= 0x10FFFF) {
        buffer[idx++] = static_cast<uint8_t>(0xF0 | (cp >> 18));
        buffer[idx++] = static_cast<uint8_t>(0x80 | ((cp >> 12) & 0x3F));
        buffer[idx++] = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3F));
        buffer[idx++] = static_cast<uint8_t>(0x80 | (cp & 0x3F));
    }
    points +=  1;
    bytes += cpBytes;
}

UTF8String operator+(const UTF8String &lhs,const UTF8String &rhs){
    size_t newSize = lhs.points + rhs.points;
    UTF8String result;
    result.points = newSize;
    result.reserved = result.points + 1;
    result.bytes = lhs.bytes + rhs.bytes;
    result.buffer = new uint8_t[result.reserved];
    std::copy(lhs.buffer, lhs.buffer + lhs.points, result.buffer);
    std::copy(rhs.buffer, rhs.buffer + rhs.reserved, result.buffer + lhs.points);
    return result;
}

UTF8String& UTF8String::operator+=(const UTF8String& str){
    size_t newSize = bytes + str.bytes;
    if (reserved < newSize + 1)
    {
        reserved = newSize + 1;
        uint8_t *newBuffer = new uint8_t[reserved];
        std::copy(buffer, buffer + bytes, newBuffer);
        delete[] buffer;
        buffer = newBuffer;
    }
    std::copy(str.buffer, str.buffer + str.reserved, buffer + bytes);
    bytes += str.bytes;
    points += str.points;
    return *this;
}

bool operator==(const UTF8String &lhs,const UTF8String &rhs){
    if (lhs.points != rhs.points)
    {
        return false;
    }
    if (lhs.bytes != rhs.bytes)
    {
        return false;
    }
    for (size_t i = 0; i < lhs.points; i++)
    {
        if (lhs.buffer[i] != rhs.buffer[i])
        {
            return false;
        }
        
    }
    return true;
}
