#include "tasks.h"

int main() {

    const auto* a = new Integer{5};
    auto arr = new Array{{new Integer{1},new Integer{2}}};
    

    delete a;
    delete arr;


    return 0;
}
