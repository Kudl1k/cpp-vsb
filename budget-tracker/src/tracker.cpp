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
    option_win = newwin(yMax/2,xMax-2,yMax/2-2,1);
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
        else if (state == END)
        {
            break;;
        }
        
    }
}

void Tracker::welcome_screen(){
    box(main_win,0,0);
    edit::print_text_middle(main_win,0,yMax/2-6,xMax,"Budget Tracker - KUD0132",true);
    edit::print_text_middle(main_win,0,yMax/2-2,xMax,"Choose version:",true);

    std::vector options = {"Plain","Simulation"};
    int option_row = 0;
    for (int i = 0; i < options.size(); i++)
    {
        if (i == highlight)
        {
            wattron(main_win,A_REVERSE);
        }
        edit::print_text_middle(main_win,0,yMax/2+option_row,xMax,options[i],false);
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
                highlight = options.size()-1;
            } 
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight > options.size()-1)
            {
                highlight = 0;
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
        edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Enter your name: ",true);
        wrefresh(main_win);
        wgetstr(main_win, input_name);
        if (strcmp(input_name,"") == 0)
        {
            wclear(main_win);
            box(main_win, 0, 0);
            edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Invalid name. Press any key to reset.",true);
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
        edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Enter your balance: ",true);
        wrefresh(main_win);
        wgetstr(main_win, input_balance);
        balance_entered = true;

        try {
            float balance = std::stof(input_balance);
            user = new User{input_name, balance};
        } catch (const std::invalid_argument& e) {
            wclear(main_win);
            box(main_win, 0, 0);
            edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Invalid balance input. Press any key to reset.",true);
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
        edit::print_text_middle(main_win, 0, yMax / 2, xMax, "Your profile is set. Press any key to start.",true);
        wrefresh(main_win);
        wgetch(main_win);
        state = MAIN;
    }
}


void Tracker::main_screen(){
    wclear(main_win);
    noecho();
    box(main_win, 0, 0);
    
    edit::print_user_info(main_win,1,"Name:",user->getName());
    std::string bal = std::to_string(user->getBalance());
    edit::print_user_info(main_win,2,"Balance:",bal.substr(0,bal.length()-4) + " $");

    // Display message about toggling options
    if (!toggle_options) {
        std::string message = "Press 'o' to show options.";
        edit::print_text_middle(main_win,0,yMax-2,xMax,message,true);
    } else {
        std::string message = "Press 'o' to hide options.";
        edit::print_text_middle(main_win,0,yMax-2,xMax,message,true);
    }
    std::vector options = {"Add balance","Add incomes","Add expanses","Get Report"};
    
    // Clear and refresh option_win to ensure it's ready for new content
    wclear(option_win);
    if (toggle_options) {
        box(option_win,0,0);
        // Print options to option_win
        int option_row = 0;
        for (int i = 0; i < options.size(); i++)
        {
            if (i == highlight)
            {
                wattron(option_win,A_REVERSE);
            }
            edit::print_text_middle(option_win,0,yMax/2+option_row,xMax,options[i],false);
            option_row++;
            wattroff(option_win,A_REVERSE);
        }
    }
    wrefresh(main_win); // Refresh main_win to ensure it's up to date
    wrefresh(option_win); // Refresh option_win after updating its content


    int ch = wgetch(option_win);
    
    switch (ch) {
        case 'o':
            toggle_options = !toggle_options;
            // Refresh option_win after toggling options
            wrefresh(option_win);
            break;
        case 'q':
            state = END;
            break;
        case KEY_DOWN:
            // Handle down arrow key
            break;
        case '\n': // Enter key
            // Handle menu item selection
            break;
        default:
            break;
    }
    
    
    choice = wgetch(main_win);

    if (ch == 'o')
    {
        switch (choice)
        {
            case KEY_UP:
                highlight--;
                if (highlight == -1)
                {
                    highlight = options.size()-1;
                } 
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight > options.size()-1)
                {
                    highlight = 0;
                }
                break;
            default:
                break;
        }
        if (choice == 10)
        {
            wclear(option_win);
            wrefresh(option_win);    
        }
    }
    
    
}