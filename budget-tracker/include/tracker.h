#pragma once

#include "user.h"
#include "expanse.h"
// #include "ui.h"


// #include <iostream>
#include <string>
#include <vector>
// #include <iostream>
// #include <chrono>
// #include <sstream>





// enum State{
//     WELCOME,
//     SETUP,
//     MAIN,
//     END
// };



class Tracker
{
public:
    Tracker();
    ~Tracker();
    std::pair<bool, std::string> addExpanse(QDate *date, int category, int subcategory, std::string title, double value);
    std::vector<Expanse> getExpanses();
private:
    User *user = nullptr;
    std::vector<Expanse> expanses;
};


