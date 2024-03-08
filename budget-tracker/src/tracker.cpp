#include "tracker.h"



void Tracker::toggle_exit(){
    exit = !exit;
}

void Tracker::toggle_setup(){
    setup = !setup;
}

void Tracker::setup_tracker(){

}

void Tracker::print_actions(){
    std::cout << "Actions:" << std::endl;
    if (!setup)
    {
        std::cout << "- Type \033[1;31 setup \033[0m for setup tracker." << std::endl;
    }
    std::cout << "\n\n";
    std::cout << "Type here your action: ";
}

void Tracker::input_handle(){
    std::cin >> input;
}

void Tracker::tracker(){
    while (!exit)
    {
        
    }
    
}