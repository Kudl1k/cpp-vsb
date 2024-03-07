#pragma once

#include <iostream>
#include <string>

#ifdef _WIN64
#include <conio.h>
#elif _WIN32
#include <conio.h>
#endif


std::string getOsName();

void clear_console();