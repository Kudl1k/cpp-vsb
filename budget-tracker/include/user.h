#pragma once

#include <iostream>


class User
{
public:
    User(std::string name, float balance);
    ~User();
private:
    std::string name;
    float balance;
    

};


