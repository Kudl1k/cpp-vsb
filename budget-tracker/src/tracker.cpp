#include "tracker.h"


Tracker::Tracker(){
    user = new User("Franta",50000);
}

Tracker::~Tracker(){
    delete user;
}

std::pair<bool, std::string> Tracker::addExpanse(QDate date, std::string category_name,std::string subcategory_name, std::string title, double value){

    if (title.empty())
    {
        title = category_name + " - " + subcategory_name;
    }
    Expanse *e = new Expanse(date,category_name,subcategory_name,title,value);
    
    expanses.push_back(*e);
    
    return std::make_pair(true,"Expanse was successfully added");
}

std::vector<Expanse> Tracker::getExpanses()
{
    return expanses;
}

