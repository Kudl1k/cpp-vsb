#pragma once

#include <iostream>


class User
{
public:
    User(std::string name, float balance);
    ~User();

    std::string getName();
    float getBalance();

private:
    std::string name;
    float balance;
    

};


