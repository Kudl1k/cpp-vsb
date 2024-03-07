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
    std::string system = getOsName();
    if (system == "Windows 32-bit" || system == "Windows 64-bit")
    {
        /* code */
    }
    else if (system == "Mac OSX")
    {
        /* code */
    }
    else if (system == "Linux")
    {
        std::cout << "\x1B[2J\x1B[H";
    }
}