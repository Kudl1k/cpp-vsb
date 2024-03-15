#include "tasks.h"

int main() {

    const auto* a = new Integer{5};
    auto* b = a->clone();


    if (a != b)
    {
        std::cout << "Not equal and value is " << b->get_value() << std::endl;
    }
    

    delete a;
    delete b;


    return 0;
}
