#include "user.h"

User::User(std::string name, int expensePercentage, int savingsPercentage) {
    this->name = name;
    this->balance = 0;
    this->nextIncomes = 0;
    this->nextExpanses = 0;
    this->expensePercentage = expensePercentage;
    this->savingsPercentage = savingsPercentage;
}

User::~User() {}

std::string User::getName(){
    return name;
}

float User::getBalance(){
    return balance;
}

int User::getExpensePercentage()
{
    return expensePercentage;
}


int User::getSavingsPercentage()
{
    return savingsPercentage;
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
