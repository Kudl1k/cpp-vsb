#pragma once

#include "user.h"
#include "expense.h"
#include "income.h"


#include <string>
#include <vector>



class Tracker
{
public:
    Tracker();
    ~Tracker();
    std::pair<bool, std::string> addExpense(QDate date, std::string category_name,std::string subcategory_name, std::string title, double value);
    std::pair<bool, std::string> addIncome(QDate date, std::string category_name,std::string subcategory_name, std::string title, double value);
    std::map<QDate,std::vector<Expense>> getExpenses();
    std::map<QDate,std::vector<Income>> getIncomes();

    int generateNewExpenseId();
    int generateNewIncomeId();
    //void removeExpanse(int index);
private:
    User *user = nullptr;
    std::map<QDate,std::vector<Expense>> expenses;
    std::map<QDate,std::vector<Income>> incomes;
};


