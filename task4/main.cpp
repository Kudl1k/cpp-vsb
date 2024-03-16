#include "tasks.h"

int main() {

    Object object{{
        {"a", new Integer{1}},
        {"b", new Integer{3}},
        {"c", new Integer{2}},
    }};
    object.insert("d", new Integer{10});
    object.insert("e", new Array{{new Integer{11}, new Integer{12}}});


    if (object.size() == 5)
    {
        std::cout << "True" << std::endl;
    }
    if (dynamic_cast<const Array*>(object["e"])->size() == 2)
    {
        std::cout << "True" << std::endl;
    }
    
    

    return 0;
}
