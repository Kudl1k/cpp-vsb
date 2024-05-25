#include "user.h"

User::User(std::string name, int expensePercentage, int incomePercentage, int savingsPercentage, int month_goal) : name{name}, expensePercentage{expensePercentage}, incomePercentage{incomePercentage}, savingsPercentage{savingsPercentage}, month_goal{month_goal}, balance{0} {}

User::~User() {}

std::string User::getName(){
    return name;
}

float User::getBalance(){
    return balance;
}

float User::getNextIncomes()
{
    return nextIncomes;
}

float User::getNextExpanses()
{
    return nextExpanses;
}

void User::addBalance(float amount){
    balance += amount;
}

void User::removeBalance(float amount)
{
    balance -= amount;
}

void User::addNextIncome(float amount)
{
    nextIncomes += amount;
}

void User::addNextExpanses(float amount)
{
    nextExpanses += amount;
}
