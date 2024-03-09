#pragma once

#include "editor.h"
#include "user.h"


#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <stdexcept>


enum State{
    WELCOME,
    SETUP,
    MAIN
};



class Tracker
{
public:
    Tracker();
    ~Tracker();

    void tracker();

    void welcome_screen();

    void setup_screen();

private:

    WINDOW* main_win;

    int xMax = 0;
    int yMax = 0;

    int choice;
    int highlight = 0;

    bool name_entered = false;
    bool balance_entered = false;

    State state = WELCOME;

    User *user;

};


