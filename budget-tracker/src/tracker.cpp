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
    const std::vector<std::string> menu_entries = {
        "Plain",
        "Simulation",
    };
    int menu_selected = 0;
    std::vector<Component> options;
    for (const auto& entry : menu_entries) {
        options.push_back(Button(entry, [&] { setup_screen(); }));
    }
    auto layout = Container::Vertical(options);
    auto renderer = Renderer(layout, [&] {
        return vbox({
            filler(),
            hbox({
                filler(),
                vbox({
                    hcenter({text("Budget Tracker") | bold | underlined}),
                    separator(),
                    vcenter({
                        hcenter({
                            vbox({
                                options[0]->Render(),
                                options[1]->Render(),
                            }) | flex
                        }),
                        
                    })| flex,
                    filler(),
                    separator(),
                    hcenter({
                        text("Author: KUD0132")
                    })
                })  | size(WIDTH,EQUAL,40) | size(HEIGHT,EQUAL,10) | border,
                filler()
            }) | flex ,            
            filler(),
        }) | flex;
    });
    screen.Loop(renderer);
    
    
}


void Tracker::setup_screen(){
    auto back_button = Button("Back", [&] { this->welcome_screen(); });
    std::string name = "";
    auto input_name = Input(&name, "here enter your name");
    std::string balance = "";
    Component input_balance = Input(&balance, "phone number");
    input_balance |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });

    auto layout = Container::Vertical({
        input_name,
        input_balance,
        back_button
    });



    auto renderer = Renderer(layout, [&] {
        return vbox({
            filler(),
            hbox({
                filler(),
                vbox({
                    hcenter({text("Setup") | bold | underlined}),
                    separator(),
                    hbox({text("Your name: "),input_name->Render()}),
                    hbox({text("Your balance:"),input_balance->Render()}),
                    filler(),
                    vbox({
                        filler(),
                        separator(),
                        back_button->Render(),
                    })
                })  | size(WIDTH,EQUAL,40) | size(HEIGHT,LESS_THAN ,10)  | border,
                filler()
            }) | flex ,            
            filler(),
        }) | flex;
    });
    screen.Loop(renderer);
}


void Tracker::main_screen(){
    
}

void Tracker::add_balance_action(){
    
}