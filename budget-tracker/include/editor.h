#pragma once

#include <iostream>
#include <ncurses.h>
#include <cstring>

namespace edit {
    void print_text_middle(WINDOW* win, int startx, int starty, int width, const std::string& text);
    void print_user_info(WINDOW* win,int starty, const std::string& info, const std::string& text);

};