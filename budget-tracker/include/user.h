#pragma once

#include <iostream>


class User
{
public:
    User(std::string name, int expensePercentage, int savingsPercentage);
    ~User();

    std::string getName();
    float getBalance();
    int getExpensePercentage();
    int getSavingsPercentage();

    float getNextIncomes();
    float getNextExpanses();

    void addBalance(float amount);
    void removeBalance(float amount);
    
    void addNextIncome(float amount);
    void addNextExpanses(float amount);


private:
    std::string name;
    float balance;
    int expensePercentage;
    int savingsPercentage;
    float nextIncomes;
    float nextExpanses;

    

};


