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
    
    InputOption opt;
    opt.multiline = false;
    std::string name = "";
    auto input_name = Input(&name, "name",opt);
    std::string balance = "";
    Component input_balance = Input(&balance, "balance",opt);
    std::string error_message = "";

    // Clear the error message when the user starts typing

        bool next_clicked = false;

    auto next_button = Button("Next", [&] { 
        next_clicked = true;
        if(name.empty() || balance.empty()) {
            error_message = "Please fill in all fields.";
        } else {
            this->main_screen(); 
        }
    });

    input_name |= CatchEvent([&](Event event){
        if (!error_message.empty() && name.empty() && !next_clicked)
        {
            error_message = "";
        }
        next_clicked = false;
        return false;
    });

    input_balance |= CatchEvent([&](Event event) {
        if (!error_message.empty() && balance.empty() && !next_clicked)
        {
            error_message = "";
        }
        next_clicked = false;
        return event.is_character() && !std::isdigit(event.character()[0]);
    });
    
    auto back_button = Button("Back", [&] { this->welcome_screen(); });
    


    auto layout = Container::Vertical({
        input_name,
        input_balance,
        Container::Horizontal({
            next_button,
            back_button
        })
    });

    
    

    auto renderer = Renderer(layout, [&] {
        return vbox({
            hbox({
                filler(),
                vbox({
                    hcenter({text("Setup") | bold | underlined}),
                    separator(),
                    hbox({text("Your name: "),input_name->Render()}),
                    hbox({text("Your balance:"),input_balance->Render()}),
                    filler(),
                    hcenter(text(error_message) | color(Color::Red)),
                    separator(),
                    vbox({
                        hcenter({
                            hbox({
                                back_button->Render(),
                                next_button->Render(),
                            })
                        })| flex,                        
                    }),
                })  | size(WIDTH,EQUAL,40) | border,
                filler()
            }) | flex ,            
        }) | flex;
    });
    screen.Loop(renderer);
}


void Tracker::main_screen(){
    auto renderer = Renderer([&]{
        return hbox({
            text("Hello"),
        });
    });
    screen.Loop(renderer);
}

void Tracker::add_balance_action(){
    
}