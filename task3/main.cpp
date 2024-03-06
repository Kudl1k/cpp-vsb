#include "tasks.h"

int main() {


    auto parser = repeat(
        create_char_parser('a'),
        3
    );

    std::cout << "Start" << std::endl;
    std::cout << parser("a") << std::endl;
    std::cout << parser("aa") << std::endl;
    std::cout << parser("aaa") << std::endl;
    std::cout << parser("aaaa") << std::endl;

    return 0;
}
