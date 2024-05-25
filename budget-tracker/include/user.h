#pragma once

#include <iostream>


class User
{
public:
    User(std::string name, float balance);
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

    float necessary_outcomes_perct;
    float entertainment_outcomes_perct;
    float savings_perct;
    float month_goal;
    

};


