#include "tasks.h"
#include <sstream>
#include <vector>


int main() {

    auto* array = new Array{{
        new Integer{1},
        new Null{},
        new Null{},
        new Array{{new Integer{10}, new Integer{11}}},
        new Integer{4},
    }};

    std::cout << array->size() << std::endl;


    RemoveNullVisitor v;
    array->accept(v);
    std::cout << array->size() << std::endl;
    


    
    delete array;
    

    return 0;
}
