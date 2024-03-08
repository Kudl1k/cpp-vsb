#pragma once

#include <iostream>


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
    bool exit = false;
    bool setup = false;

    std::string input = "";

};


