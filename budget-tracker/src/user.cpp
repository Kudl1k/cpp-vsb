#include "user.h"



User::User(std::string name, float balance) : name{name},balance{balance} {}

User::~User(){}


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
