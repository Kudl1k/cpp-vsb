#include "tracker.h"


Tracker::Tracker(){

}

Tracker::~Tracker(){

}

void Tracker::tracker(){
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    getmaxyx(stdscr,yMax,xMax);

    WINDOW* menu_win = newwin(yMax/2,xMax/2,yMax/4,xMax/4);

    box(menu_win,0,0);
    refresh();

    keypad(menu_win, true);

    edit::print_text_middle(menu_win,0,3,xMax/2,"Budget Tracker - KUD0132");
    edit::print_text_middle(menu_win,0,6,xMax/2,"Choose version:");

    std::vector options = {"Plain","Simulation"};

    int choice;
    int highlight = 0;

    while (1)
    {
        int option_row = 8;
        for (int i = 0; i < options.size(); i++)
        {
            if (i == highlight)
            {
                wattron(menu_win,A_REVERSE);
            }
            edit::print_text_middle(menu_win,0,option_row++,xMax/2,options[i]);
            wattroff(menu_win,A_REVERSE);
        }
        choice = wgetch(menu_win);

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
            {
                highlight = 0;
            } 
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight > options.size()-1)
            {
                highlight = options.size()-1;
            }
            break;
        default:
            break;
        }
        if (choice == 10)
        {
            break;
        }
    }
    


    wrefresh(menu_win);

    

    getch();
    endwin();
}