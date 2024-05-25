#pragma once

#include <iostream>


class User
{
public:
    User(std::string name, int expensePercentage, int incomePercentage, int savingsPercentage, int month_goal);
    ~User();

    std::string getName();
    float getBalance();
    float getNextIncomes();
    float getNextExpanses();

    void addBalance(float amount);
    void removeBalance(float amount);
    
    void addNextIncome(float amount);
    void addNextExpanses(float amount);

private:
    std::string name;
    float balance;
    float nextIncomes;
    float nextExpanses;

    int expensePercentage;
    int incomePercentage;
    int savingsPercentage;
    int month_goal;
    

};


