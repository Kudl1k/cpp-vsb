#include "tracker.h"


Tracker::Tracker(){
    curtime = time(0);
    time_manager = 0;
    local_time = *localtime(&curtime);
}

Tracker::~Tracker(){
    delete user;
}

void Tracker::tracker(){
    welcome_screen();
    curtime = time(0);
}

void Tracker::welcome_screen(){
    const std::vector<std::string> menu_entries = {
        "Plain",
        "Test version",
        "Quit"
    };
    int menu_selected = 0;
    std::vector<Component> options;
    options.push_back(Button(menu_entries[0], [&] { setup_screen(); },ButtonOption::Ascii()));
    options.push_back(Button(menu_entries[1], [&] {
        this->user = new User("Test User",50000);
        main_screen();
    },ButtonOption::Ascii()));
    options.push_back(Button(menu_entries[2], [&] {
        user->~User();
        screen.Exit();
    },ButtonOption::Ascii()));

    auto layout = Container::Vertical(options);
    
    auto renderer = ui::create_welcomescreen(layout,options);
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
            this->user = new User(name,std::stof(balance));
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
    
    auto back_button = Button("Back", [&] { 
        this->welcome_screen(); 
    },ButtonOption::Ascii());
    
    int neceserry = 50;
    auto neceserry_slider = Slider("", &neceserry, 30, 60, 1);

    int entertainment = 30;
    auto entertainment_slider = Slider("", &entertainment, 10, 40, 1);

    auto layout = Container::Vertical({
        input_name,
        input_balance,
        neceserry_slider,
        entertainment_slider,
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
                    separator(),
                    hbox({text("Necessary:"),text(std::to_string(neceserry) + "%"),neceserry_slider->Render()}),
                    hbox({text("Entertainment:"),text(std::to_string(entertainment) + "%"),entertainment_slider->Render()}),
                    hcenter({
                        hbox({
                            text("Info: ")|bold,
                            text("Rest of the percentage is for saving")
                        })
                    }),
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
                })  | size(WIDTH,EQUAL,60) | border,
                filler()
            }) | flex ,
            filler()            
        }) | flex;
    });
    screen.Loop(renderer);
}


void Tracker::main_screen(){
    curtime = time(0) + time_manager;
    local_time = *localtime(&curtime);
    std::vector<std::string> tab_values{
        "Main panel",
        "This week",
        "This month",
    };
    int tab_selected = 0;
    auto tab_toggle = Toggle(&tab_values, &tab_selected);
    auto tab_container = Container::Tab({
        ui::create_calendar(&local_time)
    },&tab_selected);

    auto button = Button("x",[&]{ 
        screen.Exit();
    },ButtonOption::Ascii());

    auto add_day_button = Button("Add day",[&]{
        time_manager += 86400;
    },ButtonOption::Ascii());

    auto remove_day_button = Button("Remove day",[&]{
        time_manager -= 86400;
    },ButtonOption::Ascii());

    auto container = Container::Vertical({
        Container::Horizontal({
            tab_toggle,
            add_day_button,
            remove_day_button,
            button,
        }),
        tab_container,
    });

    
    auto renderer = Renderer(container, [&] {
        curtime = time(0) + time_manager;
        local_time = *localtime(&curtime);
        char buff[20];
        strftime(buff, 20, "%Y-%m-%d %H:%M", &local_time);
        std::string balance = std::to_string(user->getBalance());
        return vbox({
            hbox({
            tab_toggle->Render(),
            filler(),
            text(user->getName() + "  "),
            text(balance.substr(0,balance.size()-4) + "kč  "),
            filler(),
            text(buff),
            filler(),
            add_day_button->Render(),
            remove_day_button->Render(),
            button->Render()
            }),
            separator(),
            tab_container->Render(),
        }) | border;
    });
    
    screen.Loop(renderer);
}

void Tracker::main_panel(){
    
}

void Tracker::add_balance_action(){
    
}