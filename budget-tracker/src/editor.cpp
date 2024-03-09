#include "editor.h"


void edit::print_text_middle(WINDOW* win, int startx, int starty, int width, const std::string& text){

    int y, x;
    getyx(win, y, x);

    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;

    if (width == 0)
        width = 80;

    int length = text.length();
    int temp = (width - length) / 2;
    x = startx + temp;
    mvwprintw(win, y, x, "%s", text.c_str());
}

void edit::print_user_info(WINDOW* win, int starty, const std::string& info, const std::string& text) {
    int startx_label = 1;
    int startx_text = 20;
    mvwprintw(win, starty, startx_label,"%s", info.c_str());
    mvwprintw(win, starty, startx_text,"%s",text.c_str());
}