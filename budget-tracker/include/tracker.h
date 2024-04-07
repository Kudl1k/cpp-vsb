#pragma once

#include "user.h"
#include "expanse.h"
#include "income.h"


#include <string>
#include <vector>



class Tracker
{
public:
    Tracker();
    ~Tracker();
    std::pair<bool, std::string> addExpanse(QDate date, std::string category_name,std::string subcategory_name, std::string title, double value);
    std::pair<bool, std::string> addIncome(QDate date, std::string category_name,std::string subcategory_name, std::string title, double value);
    std::vector<Expanse> getExpanses();
    std::vector<Income> getIncomes();
    //void removeExpanse(int index);
private:
    User *user = nullptr;
    std::vector<Expanse> expanses;
    std::vector<Income> incomes;
};


