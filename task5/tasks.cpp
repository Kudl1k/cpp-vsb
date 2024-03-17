#include "tasks.h"



UTF8String::UTF8String(const std::string& s) : buffer(std::vector<uint8_t>()){
    points = s.length();
    bytes = points;
    for (size_t i = 0; i < points; i++)
    {
        buffer.push_back(static_cast<uint8_t>(s[i]));
    }
}

UTF8String::UTF8String(const std::vector<CodePoint>& ptr) : buffer(std::vector<uint8_t>()){
    points = ptr.size();
    for (size_t i = 0; i < points; i++)
    {
        bytes += get_number_of_bytes(ptr[i]);
    }

    for (size_t i = 0; i < points;i++)
    {
        if (ptr[i] <=   0x7F) {
            buffer.push_back(static_cast<uint8_t>(ptr[i]));
        } else if (ptr[i] <=   0x7FF) {
            buffer.push_back(static_cast<uint8_t>(0xC0 | (ptr[i] >>   6)));
            buffer.push_back(static_cast<uint8_t>(0x80 | (ptr[i] &   0x3F)));
        } else if (ptr[i] <=   0xFFFF) {
            buffer.push_back(static_cast<uint8_t>(0xE0 | (ptr[i] >>   12)));
            buffer.push_back(static_cast<uint8_t>(0x80 | ((ptr[i] >>   6) &   0x3F)));
            buffer.push_back(static_cast<uint8_t>(0x80 | (ptr[i] &   0x3F)));
        } else if (ptr[i] <=   0x10FFFF) {
            buffer.push_back(static_cast<uint8_t>(0xF0 | (ptr[i] >>   18)));
            buffer.push_back(static_cast<uint8_t>(0x80 | ((ptr[i] >>   12) &   0x3F)));
            buffer.push_back(static_cast<uint8_t>(0x80 | ((ptr[i] >>   6) &   0x3F)));
            buffer.push_back(static_cast<uint8_t>(0x80 | (ptr[i] &   0x3F)));
        }
    }
}

UTF8String::UTF8String(const UTF8String &ptr){
    points = ptr.points;
    bytes = ptr.bytes;
    buffer = ptr.buffer;
}

UTF8String::UTF8String(UTF8String &&other)
{
    points = other.points;
    bytes = other.bytes;
    buffer = std::move(other.buffer);
    other.points = 0;
    other.bytes = 0;
}

UTF8String& UTF8String::operator=(const UTF8String &ptr){
    points = ptr.points;
    bytes = ptr.bytes;
    buffer = ptr.buffer;
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

std::optional<uint8_t> UTF8String::operator[](const size_t idx) const{
    if (idx >= bytes)
    {
        return std::nullopt;   
    } else {
        return buffer.at(idx);
    }
}

std::optional<CodePoint> UTF8String::nth_code_point(const size_t idx) const {
    if (idx >= points){
        return std::nullopt;
    }

    size_t byteIdx = 0;
    size_t pointCount = 0;

    while (byteIdx < bytes && pointCount < idx) {
        uint8_t firstByte = buffer.at(byteIdx);
        if ((firstByte & 0x80) == 0) {
            ++pointCount;
            ++byteIdx;
        } else if ((firstByte & 0xE0) == 0xC0) {
            if (byteIdx + 1 < bytes) {
                ++pointCount;
                byteIdx += 2;
            } else {
                return std::nullopt;
            }
        } else if ((firstByte & 0xF0) == 0xE0) {
            // Three byte character
            if (byteIdx + 2 < bytes) {
                ++pointCount;
                byteIdx += 3;
            } else {
                return std::nullopt;
            }
        } else if ((firstByte & 0xF8) == 0xF0) {
            if (byteIdx + 3 < bytes) {
                ++pointCount;
                byteIdx += 4;
            } else {
                return std::nullopt;
            }
        } else {
            return std::nullopt;
        }
    }

    if (pointCount == idx) {
        if ((buffer.at(byteIdx) & 0x80) == 0) {
            return buffer.at(byteIdx);
        } else if ((buffer.at(byteIdx) & 0xE0) == 0xC0) {
            return ((buffer.at(byteIdx) & 0x1F) << 6) | (buffer.at(byteIdx + 1) & 0x3F);
        } else if ((buffer.at(byteIdx) & 0xF0) == 0xE0) {
            return ((buffer.at(byteIdx) & 0x0F) << 12) | ((buffer.at(byteIdx + 1) & 0x3F) << 6) | (buffer.at(byteIdx + 2) & 0x3F);
        } else if ((buffer.at(byteIdx) & 0xF8) == 0xF0) {
            return ((buffer.at(byteIdx) & 0x07) << 18) | ((buffer.at(byteIdx + 1) & 0x3F) << 12) | ((buffer.at(byteIdx + 2) & 0x3F) << 6) | (buffer.at(byteIdx + 3) & 0x3F);
        }
    }

    return std::nullopt;
}

void UTF8String::append(const char c){
    buffer.push_back(static_cast<CodePoint>(c));
    points +=  1;
    bytes += 1;
}

void UTF8String::append(const CodePoint cp){
    size_t cpBytes = get_number_of_bytes(cp);
    size_t idx = bytes;
    if (cp <=   0x7F) {
        buffer.push_back(static_cast<uint8_t>(cp));
    } else if (cp <=   0x7FF) {
        buffer.push_back(static_cast<uint8_t>(0xC0 | (cp >>   6)));
        buffer.push_back(static_cast<uint8_t>(0x80 | (cp &   0x3F)));
    } else if (cp <=   0xFFFF) {
        buffer.push_back(static_cast<uint8_t>(0xE0 | (cp >>   12)));
        buffer.push_back(static_cast<uint8_t>(0x80 | ((cp >>   6) &   0x3F)));
        buffer.push_back(static_cast<uint8_t>(0x80 | (cp &   0x3F)));
    } else if (cp <=   0x10FFFF) {
        buffer.push_back(static_cast<uint8_t>(0xF0 | (cp >>   18)));
        buffer.push_back(static_cast<uint8_t>(0x80 | ((cp >>   12) &   0x3F)));
        buffer.push_back(static_cast<uint8_t>(0x80 | ((cp >>   6) &   0x3F)));
        buffer.push_back(static_cast<uint8_t>(0x80 | (cp &   0x3F)));
    }
    points +=  1;
    bytes += cpBytes;
}

UTF8String operator+(const UTF8String &lhs,const UTF8String &rhs){
    size_t newSize = lhs.points + rhs.points;
    UTF8String result;
    result.points = newSize;
    result.bytes = lhs.bytes + rhs.bytes;
    result.buffer.reserve(lhs.buffer.size() + rhs.buffer.size());
    result.buffer.insert( result.buffer.end(), lhs.buffer.begin(), lhs.buffer.end());
    result.buffer.insert( result.buffer.end(), rhs.buffer.begin(), rhs.buffer.end());
    return result;
}

UTF8String& UTF8String::operator+=(const UTF8String& str){
    buffer.insert(buffer.end(),str.buffer.begin(),str.buffer.end());
    bytes += str.bytes;
    points += str.points;
    return *this;
}

UTF8String &UTF8String::operator=(UTF8String &&other){
    if (this != &other) {
        points = other.points;
        bytes = other.bytes;
        buffer = std::move(other.buffer);
        other.points = 0;
        other.bytes = 0;
    }
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

bool operator!=(const UTF8String &lhs, const UTF8String &rhs)
{
    return !(lhs == rhs);
}
