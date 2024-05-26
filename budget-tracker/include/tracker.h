#pragma once

#include "user.h"
#include "expense.h"
#include "income.h"


#include <string>
#include <vector>
#include <fstream>



class Tracker
{
public:
    Tracker(User *user);
    ~Tracker();
    std::pair<bool, std::string> addExpense(QDate date, int category_name, int subcategory_name, std::string title, double value);
    std::pair<bool, std::string> addIncome(QDate date, int category_name,int subcategory_name, std::string title, double value);
    std::map<QDate,std::vector<Expense>> getExpenses();
    std::map<QDate,std::vector<Expense>> getFutureExpenses();
    std::map<QDate,std::vector<Expense>> getAllExpenses();
    std::map<QDate,std::vector<Income>> getIncomes();
    std::map<QDate,std::vector<Income>> getFutureIncomes();
    std::map<QDate,std::vector<Income>> getAllIncomes();


    int generateNewExpenseId();
    int generateNewIncomeId();
    User* getUser();

    float getIncomesSum();
    float getExpensesSum();

    float getExpensesPercentage();

    void saveToFile();
    void loadFromFile();
    //void removeExpanse(int index);
private:

    void saveUserToFile();
    void saveExpensesToFile();
    void saveIncomesToFile();


    void loadUserFromFile();
    void loadExpensesFromFile();
    void loadIncomesFromFile();

    

    User *user;
    std::map<QDate,std::vector<Expense>> expenses;
    std::map<QDate,std::vector<Expense>> futureExpanses;
    std::map<QDate,std::vector<Expense>> allExpenses;
    std::map<QDate,std::vector<Income>> incomes;
    std::map<QDate,std::vector<Income>> futureIncomes;
    std::map<QDate,std::vector<Income>> allIncomes;



};


