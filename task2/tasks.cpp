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

UTF8String::UTF8String(std::vector<CodePoint>){

}

int UTF8String::get_number_of_bytes(CodePoint *ptr){
    
    return 0;
}


int UTF8String::get_point_count(){
    return points;
}

int UTF8String::get_byte_count(){
    return bytes;
}
