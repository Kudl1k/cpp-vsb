#include "utils.h"


std::string getOsName() {
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}

void clear_console(){
    std::string system_name = getOsName();
    if (system_name == "Windows 32-bit" || system_name == "Windows 64-bit")
    {
        #ifdef _WIN64
        system(cls);
        #endif
        #ifdef _WIN32
        system(cls);
        #endif
    }
    else if (system_name == "Mac OSX")
    {
        /* code */
    }
    else if (system_name == "Linux")
    {
        std::cout << "\x1B[2J\x1B[H";
    }
}