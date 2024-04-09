#include "tracker.h"


Tracker::Tracker(){
    user = new User("Franta",50000);
}

Tracker::~Tracker(){
    delete user;
}

User* Tracker::getUser(){
    return user;
}

std::pair<bool, std::string> Tracker::addExpense(QDate date, int category_name, int subcategory_name, std::string title, double value){

    if (title == "")
    {
        title = category_name + " - " + subcategory_name;
    }
    Expense *e = new Expense(date,category_name,subcategory_name,title,value);
    
    expenses[date].push_back(*e);
    
    return std::make_pair(true,"Expense was successfully added");
}

std::pair<bool, std::string> Tracker::addIncome(QDate date, int category_name,int subcategory_name, std::string title, double value){

    if (title.empty())
    {
        title = category_name + " - " + subcategory_name;
    }
    Income *i = new Income(date,category_name,subcategory_name,title,value);
    
    incomes[date].push_back(*i);
    
    return std::make_pair(true,"Expense was successfully added");
}

std::map<QDate,std::vector<Expense>> Tracker::getExpenses()
{
    return expenses;
}

std::map<QDate,std::vector<Income>> Tracker::getIncomes()
{
    return incomes;
}

int Tracker::generateNewExpenseId()
{
    return expenses.size();
}

int Tracker::generateNewIncomeId()
{
    return incomes.size();
}