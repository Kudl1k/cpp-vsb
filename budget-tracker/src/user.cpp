#include "user.h"



User::User(std::string name, float balance)
{
    this->name = name;
    this->balance = balance;
}

User::~User()
{
}


std::string User::getName(){
    return name;
}

float User::getBalance(){
    return balance;
}

void User::addBalance(float amount){
    balance += amount;
}