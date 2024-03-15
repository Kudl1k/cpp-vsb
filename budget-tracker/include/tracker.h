#pragma once

#include "user.h"
#include "ui.h"


#include <iostream>
#include <string>
#include <vector>





enum State{
    WELCOME,
    SETUP,
    MAIN,
    END
};



class Tracker
{
public:
    Tracker();
    ~Tracker();

    void tracker();

    void welcome_screen();
    void setup_screen();
    void main_screen();


    void add_balance_action();

private:


    int xMax = 0;
    int yMax = 0;

    int choice;
    int highlight = 0;

    bool name_entered = false;
    bool balance_entered = false;

    bool toggle_options = false;
    bool action_toggle = false;

    State state = WELCOME;

    User *user;

};


