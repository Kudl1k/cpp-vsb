#pragma once

#include <editor.h>

#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>


class Tracker
{
public:
    Tracker();
    ~Tracker();

    void tracker();

    void toggle_exit();
    void toggle_setup();
    void setup_tracker();
    void print_actions();

    void input_handle();

private:

    int xMax = 0;
    int yMax = 0;


    bool exit = false;
    bool setup = false;

    std::string input = "";

};


