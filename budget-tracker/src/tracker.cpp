#include "tracker.h"

/*
Default constructor for setting up the budget tracker.
Initialize the terminal state by ncurses library.
Disabling the cursor and obtaining the maximum values for x and y sides.
Allowing arrowkeys for movement in terminal.
*/
Tracker::Tracker(){
    initscr();
    cbreak();
    curs_set(0);
    getmaxyx(stdscr,yMax,xMax);
    main_win = newwin(yMax,xMax,0,0);
    box(main_win,0,0);
    refresh();
    keypad(main_win, true);
}



/*
Deconstructor of Tracker class
Disabling the terminal state.
*/
Tracker::~Tracker(){
    delete user;
    endwin();
}

void Tracker::tracker(){
    while (1)
    {
        if (state == WELCOME)
        {
            welcome_screen();
        }
        else if (state == SETUP && highlight == 0)
        {
            setup_screen();
        }
        else if (state == MAIN)
        {
            main_screen();
        }
            
    }
    wrefresh(main_win);
    getch();
    
}

void Tracker::welcome_screen(){
    box(main_win,0,0);
    edit::print_text_middle(main_win,0,yMax/2-6,xMax,"Budget Tracker - KUD0132");
    edit::print_text_middle(main_win,0,yMax/2-2,xMax,"Choose version:");

    std::vector options = {"Plain","Simulation"};
    int option_row = 0;
    for (int i = 0; i < options.size(); i++)
    {
        if (i == highlight)
        {
            wattron(main_win,A_REVERSE);
        }
        edit::print_text_middle(main_win,0,yMax/2+option_row,xMax,options[i]);
        option_row++;
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
        if (highlight == 0)
        {
            state = SETUP;
        }
        else {
            state = MAIN;
            user = new User{"Simulation User",300};
        }
        
    }
}


void Tracker::setup_screen(){
    curs_set(1);
    box(main_win, 0, 0);
    
    char input_name[20];
    char input_balance[20];

    

    
    if (!name_entered)
    {
        wclear(main_win);
        box(main_win, 0, 0);
        edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Enter your name: ");
        wrefresh(main_win);
        wgetstr(main_win, input_name);
        if (strcmp(input_name,"") == 0)
        {
            wclear(main_win);
            box(main_win, 0, 0);
            edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Invalid name. Press any key to reset.");
            wrefresh(main_win);
            wgetch(main_win);
        }
        else{
            name_entered = true;

        }
        
    }
    if (!balance_entered && name_entered)
    {
        wclear(main_win);
        box(main_win, 0, 0);
        edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Enter your balance: ");
        wrefresh(main_win);
        wgetstr(main_win, input_balance);
        balance_entered = true;

        try {
            float balance = std::stof(input_balance);
            user = new User{input_name, balance};
        } catch (const std::invalid_argument& e) {
            wclear(main_win);
            box(main_win, 0, 0);
            edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Invalid balance input. Press any key to reset.");
            wrefresh(main_win);
            wgetch(main_win);
            balance_entered = false;
        }
    }
    if (balance_entered && name_entered)
    {
        float bal = std::atof(input_balance);
        user = new User{input_name,bal};
        curs_set(0);
        noecho();
        wclear(main_win);
        box(main_win, 0, 0);
        edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Your profile is set. Press any key to start.");
        wrefresh(main_win);
        wgetch(main_win);
        state = MAIN;
    }
}


void Tracker::main_screen(){
    wclear(main_win);
    box(main_win, 0, 0);
    
    edit::print_user_info(main_win,1,"Name:",user->getName());
    std::string bal = std::to_string(user->getBalance());
    edit::print_user_info(main_win,2,"Balance:",bal.substr(0,bal.length()-4) + " $");

    



    wrefresh(main_win);

    wgetch(main_win);
}