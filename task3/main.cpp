#include "tasks.h"

int main() {


    auto parser1 = create_char_parser('x');

    const char* x = "x";

    std::cout << parser1("x") << std::endl;
    if (x+1 == nullptr)
    {
        std::cout << "null" << std::endl;
    }
    if (parser1("x") == nullptr)
    {
        std::cout << "x is null" << std::endl;
    }
    
    
    std::cout << x+1 << std::endl;

    return 0;
}
