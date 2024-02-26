#include "tasks.h"

int main() {

    UTF8String str { "ahoj" };

    UTF8String str2 {
        std::vector<CodePoint>{
            0x1F601, // 😁
            0x1F614, // 😔
            static_cast<CodePoint>('0'),
            0x010D // č
        } 
    };
    std::cout << str.get_byte_count() << std::endl; 
    std::cout << str2.get_byte_count() << " " << str2.get_point_count() << std::endl;
    return 0;
}
