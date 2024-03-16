#include "tasks.h"

int main() {

    auto* array = new Array{{
        new Integer{1},
        new Null{},
        new Null{},
        new Array{{new Integer{10}, new Integer{11}}},
        new Integer{4},
    }};
    
    delete array;
    

    return 0;
}
