#pragma once

#include "editor.h"
#include "user.h"


#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>


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

    WINDOW* main_win;
    WINDOW* option_win;
    WINDOW* action_win;

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


