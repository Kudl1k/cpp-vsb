#include "tasks.h"



int main() {

    UTF8String str1 { "aaaa" };
    UTF8String str2 { "bbbb" };
    UTF8String str3 = str1 + str2;
    for (int i = 0; i < str3.get_point_count(); i++)
    {
        std::cout << str3[i].value() << std::endl;
    }
    
    
    return 0;
}
