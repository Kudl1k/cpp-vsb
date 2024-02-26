#include "tasks.h"



int main() {

    UTF8String str { 
        std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        } 
    };


    std::cout << "Bytes: " << str.get_byte_count() << std::endl;
    std::cout << "Points: " << str.get_point_count() << std::endl;
    for (int i =  0; i < str.get_byte_count(); i++) {
    auto byte = str[i];
    if (byte.has_value()) {
        std::cout << static_cast<int>(byte.value()) << " ";
    } else {
        std::cout << "No value ";
    }
}
    



    return 0;
}
