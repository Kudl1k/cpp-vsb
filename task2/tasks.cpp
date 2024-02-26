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
    reserved = points + 1;
    for (size_t i = 0; i < points; i++)
    {
        buffer[i] = static_cast<CodePoint>(ptr[i]);
    }
}

UTF8String::UTF8String(std::vector<CodePoint> ptr){
    points = ptr.size();
    buffer = new CodePoint[points];
    reserved = points + 1;
    for (size_t i = 0; i < points; i++)
    {
        buffer[i] = ptr[i];
        bytes += get_number_of_bytes(ptr[i]);
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


int UTF8String::get_point_count(){
    return points;
}

int UTF8String::get_byte_count(){
    return bytes;
}
