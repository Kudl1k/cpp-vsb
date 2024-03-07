#include "tasks.h"

int main() {


    auto parser1 = skip_ws();

    std::cout << parser1("   123") << std::endl;

    return 0;
}
