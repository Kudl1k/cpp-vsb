#include "tasks.h"



int main() {

    UTF8String string;
    string.append('A');
    string.append('h');
    string.append('o');
    string.append('j');
    string.append('.');
    string.append(static_cast<CodePoint>(0x1F601));
    string.append('d');
    string.append('d');
    string.append(static_cast<CodePoint>(0x1F601));
    string.append('a');


    std::cout << string.get_byte_count() << std::endl;
    std::cout << string.get_point_count() << std::endl;
    std::cout << string.get_reserved_count() << std::endl;
    for (int i = 0; i < string.get_byte_count(); ++i) {
        std::cout << string[i].value() << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < string.get_point_count(); ++i) {
        std::cout << string.nth_code_point(i).value() << std::endl;
    }

    return 0;
}
