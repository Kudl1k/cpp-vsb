#include "tracker.h"


Tracker::Tracker(){
    user = new User("Franta",50000);
}

Tracker::~Tracker(){
    delete user;
}

std::pair<bool, std::string> Tracker::addExpanse(QDate * date, int category, int subcategory, std::string title, double value){
    if (category == -1)
    {
        return std::make_pair(false, "Please select category");
    }
    if (subcategory == -1)
    {
        return std::make_pair(false, "Please select subcategory");
    }

    std::string category_name = categories[category].first;
    std::string subcategory_name = categories[category].second[subcategory];


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

