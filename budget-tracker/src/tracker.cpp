#include "tracker.h"

/*
Default constructor for setting up the budget tracker.
Initialize the terminal state by ncurses library.
Disabling the cursor and obtaining the maximum values for x and y sides.
Allowing arrowkeys for movement in terminal.
*/
Tracker::Tracker(){
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    getmaxyx(stdscr,yMax,xMax);
    main_win = newwin(yMax/2,xMax/2,yMax/4,xMax/4);
    box(main_win,0,0);
    refresh();
    keypad(main_win, true);
}



/*
Deconstructor of Tracker class
Disabling the terminal state.
*/
Tracker::~Tracker(){
    endwin();
}

void Tracker::tracker(){
    while (1)
    {
        if (state == WELCOME)
        {
            welcome_screen();
        }
        else if (state == SETUP)
        {
            
        }    
    }
    wrefresh(main_win);
    getch();
    
}

void Tracker::welcome_screen(){
    edit::print_text_middle(main_win,0,3,xMax/2,"Budget Tracker - KUD0132");
    edit::print_text_middle(main_win,0,6,xMax/2,"Choose version:");

    std::vector options = {"Plain","Simulation"};
    int option_row = 8;
    for (int i = 0; i < options.size(); i++)
    {
        if (i == highlight)
        {
            wattron(main_win,A_REVERSE);
        }
        edit::print_text_middle(main_win,0,option_row++,xMax/2,options[i]);
        wattroff(main_win,A_REVERSE);
    }
    choice = wgetch(main_win);

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
        wclear(main_win);
        wrefresh(main_win);
        state = SETUP;
    }
}
