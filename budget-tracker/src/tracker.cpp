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
            setup_screen();
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
        state = SETUP;
    }
}


void Tracker::setup_screen(){
    cbreak();
    curs_set(1);
    box(main_win,0,0);
    
    char input[20];

    std::string input_name = "";
    float input_balance = 0;


    bool name = false;
    bool balance = false;

    
    if (!name)
    {
        edit::print_text_middle(main_win,0,yMax/2-2,xMax,"Enter your name:");
        wrefresh(main_win);
        wgetstr(main_win,input);
        if (input != "")
        {
            input_name = input;
            name = true;
        }   
    }
    else if (!balance)
    {
        edit::print_text_middle(main_win,0,yMax/2-2,xMax,"Enter your balance:");
        wrefresh(main_win);
        wgetstr(main_win,input);
        if (input != "")
        {
            try {
                input_balance = std::stof(input);
                balance = true;
            } catch (const std::invalid_argument& e) {
                // Handle the case where the input is not a valid floating-point number
                std::cout << "Error: Invalid input. Please enter a number." << std::endl;
                // Optionally, you can clear the input or prompt the user again
            } catch (const std::out_of_range& e) {
                // Handle the case where the input is a number but out of the range of representable values
                std::cout << "Error: The number is out of range." << std::endl;
            }
        }  
    }
    
    


    wrefresh(main_win);


}