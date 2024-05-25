#include "user.h"

User::User(std::string name, int expensePercentage, int incomePercentage, int savingsPercentage, int month_goal) {
    this->name = name;
    this->balance = 0;
    this->nextIncomes = 0;
    this->nextExpanses = 0;
    this->expensePercentage = expensePercentage;
    this->incomePercentage = incomePercentage;
    this->savingsPercentage = savingsPercentage;
    this->month_goal = month_goal;
}

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
