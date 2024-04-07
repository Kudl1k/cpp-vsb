#include "tracker.h"


Tracker::Tracker(){
    user = new User("Franta",50000);
}

Tracker::~Tracker(){
    delete user;
}

std::pair<bool, std::string> Tracker::addExpense(QDate date, std::string category_name,std::string subcategory_name, std::string title, double value){

    if (title.empty())
    {
        title = category_name + " - " + subcategory_name;
    }
    Expense *e = new Expense(date,category_name,subcategory_name,title,value);
    
    expenses.push_back(*e);
    
    return std::make_pair(true,"Expense was successfully added");
}

std::pair<bool, std::string> Tracker::addIncome(QDate date, std::string category_name,std::string subcategory_name, std::string title, double value){

    if (title.empty())
    {
        title = category_name + " - " + subcategory_name;
    }
    Income *i = new Income(date,category_name,subcategory_name,title,value);
    
    incomes.push_back(*i);
    
    return std::make_pair(true,"Expense was successfully added");
}

std::vector<Expense> Tracker::getExpenses()
{
    return expenses;
}

std::vector<Income> Tracker::getIncomes()
{
    return incomes;
}


