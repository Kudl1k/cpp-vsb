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
    auto screen = ScreenInteractive::Fullscreen();
 
    const std::vector<std::string> menu_entries = {
        "Plain",
        "Simulation",
    };
    int menu_selected = 0;
    auto menu = Menu(&menu_entries, &menu_selected);

    auto main_menu = Renderer(menu,[menu]{
        return vbox({
            filler(),
            hbox({
                filler(),
                vbox({
                    filler(),
                    hcenter({
                        text("Budget Tracker") | bold
                    }),
                    vbox({
                        filler(),
                        hcenter({
                            text("Choose your version:") | underlined
                        }),
                        filler(),
                        hcenter({
                            menu->Render()
                        }),
                    }) | flex,
                    filler(),
                    hcenter({
                        text("Created by: KUD0132") | bold
                    })
                }) | flex | border,
                filler()
            }) | flex,
            filler()
        }) | flex;
    });

    screen.Loop(main_menu);
}


void Tracker::setup_screen(){
    
}


void Tracker::main_screen(){
    
}

void Tracker::add_balance_action(){
    
}