#include "tracker.h"

/*
Default constructor for setting up the budget tracker.
Initialize the terminal state by ncurses library.
Disabling the cursor and obtaining the maximum values for x and y sides.
Allowing arrowkeys for movement in terminal.
*/
Tracker::Tracker(){
}



/*
Deconstructor of Tracker class
Disabling the terminal state.
*/
Tracker::~Tracker(){

}

void Tracker::tracker(){
    welcome_screen();
}

void Tracker::welcome_screen(){

    Element document =
    hbox({
        text("left")   | border,
        text("middle") | border | flex,
        text("right")  | border,
    });

    auto screen = Screen::Create(
    Dimension::Full(),       // Width
    Dimension::Fit(document) // Height
    );
    Render(screen, document);
    screen.Print();
    getchar();
}


void Tracker::setup_screen(){
    
}


void Tracker::main_screen(){
    
}

void Tracker::add_balance_action(){
    
}