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
        "Test version",
    };
    int menu_selected = 0;
    std::vector<Component> options;
    options.push_back(Button(menu_entries[0], [&] { setup_screen(); },ButtonOption::Ascii()));
    options.push_back(Button(menu_entries[1], [&] { main_screen(); },ButtonOption::Ascii()));
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
                                hcenter({options[0]->Render()}),
                                hcenter({options[1]->Render()})
                            }) | flex
                        }),
                        
                    })| flex,
                    filler(),
                    separator(),
                    hcenter({
                        text("Author: KUD0132")
                    })
                })  | size(WIDTH,EQUAL,40) | border,
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
    },ButtonOption::Ascii());

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
    
    auto back_button = Button("Back", [&] { this->welcome_screen(); },ButtonOption::Ascii());
    


    auto layout = Container::Vertical({
        input_name,
        input_balance,
        Container::Horizontal({
            back_button,
            next_button
        })
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
                    hcenter(text(error_message) | color(Color::Red)),
                    vbox({
                        separator(),
                        hcenter({
                            hbox({
                                back_button->Render(),
                                next_button->Render(),
                            })
                        }),                        
                    }),
                })  | size(WIDTH,EQUAL,40) | border,
                filler()
            }) | flex ,
            filler()            
        }) | flex;
    });
    screen.Loop(renderer);
}


void Tracker::main_screen(){
    std::vector<std::string> tab_values{
        "Main panel",
        "This week",
        "This month",
    };
    int tab_selected = 0;
    auto tab_toggle = Toggle(&tab_values, &tab_selected);

    auto tab_container = Container::Tab({

    },&tab_selected);

    auto button = Button("x",[&]{ 
        screen.Exit();
    },ButtonOption::Ascii());

    auto container = Container::Vertical({
        tab_toggle,
        button,
        tab_container,
    });

    
    auto renderer = Renderer(container, [&] {
    return vbox({
        hbox({
           tab_toggle->Render(),
           filler(),
           button->Render()
        }),
        separator(),
        text(tab_values[tab_selected]),
        tab_container->Render(),
    }) | border;
    });
    
    screen.Loop(renderer);
}

void Tracker::main_panel(){
    
}

void Tracker::add_balance_action(){
    
}