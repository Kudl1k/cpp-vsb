#include "tasks.h"



UTF8String::~UTF8String(){
    if (buffer != nullptr)
    {
        delete[] buffer;
    }
    
}


UTF8String::UTF8String(const char* ptr){
    size = strlen(ptr);
    reserved = size + 1;
    buffer = new CodePoint[reserved];
    std::copy(ptr,ptr+reserved,buffer);
}

std::ostream& operator<<(std::ostream& os, const UTF8String& str) {
    if (str.buffer == nullptr) {
        return os;
    }
    return os << str.buffer;
}

