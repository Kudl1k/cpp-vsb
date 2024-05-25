#include "tracker.h"


Tracker::Tracker(User *user){
    if (user == nullptr)
    {
        this->user = new User("User", 50, 30, 20, 5000);
    } else {
        this->user = user;
    }
}

Tracker::~Tracker(){
    delete user;
}

User* Tracker::getUser(){
    return user;
}

void Tracker::saveToFile()
{
    
}

std::pair<bool, std::string> Tracker::addExpense(QDate date, int category_name, int subcategory_name, std::string title, double value){

    if (title == "")
    {
        title = category_name + " - " + subcategory_name;
    }
    Expense *e = new Expense(date,category_name,subcategory_name,title,value);
    
    if (e->getDate() <= QDate::currentDate())
    {
        expenses[date].push_back(*e);
        user->removeBalance(e->getValue());
    } else {
        futureExpanses[date].push_back(*e);
        user->addNextExpanses(e->getValue());
    }
    return std::make_pair(true,"Expense was successfully added");
}

std::pair<bool, std::string> Tracker::addIncome(QDate date, int category_name,int subcategory_name, std::string title, double value){

    if (title.empty())
    {
        title = category_name + " - " + subcategory_name;
    }
    Income *i = new Income(date,category_name,subcategory_name,title,value);
    
    if (i->getDate() <= QDate::currentDate())
    {
        incomes[date].push_back(*i);
        user->addBalance(i->getValue());
    } else {
        futureIncomes[date].push_back(*i);
        user->addNextIncome(i->getValue());
    }
    
    return std::make_pair(true,"Income was successfully added");
}

std::map<QDate,std::vector<Expense>> Tracker::getExpenses()
{
    return expenses;
}

std::map<QDate, std::vector<Expense>> Tracker::getFutureExpenses()
{
    return futureExpanses;
}

std::map<QDate,std::vector<Income>> Tracker::getIncomes()
{
    return incomes;
}

std::map<QDate, std::vector<Income>> Tracker::getFutureIncomes()
{
    return futureIncomes;
}

int Tracker::generateNewExpenseId()
{
    return expenses.size();
}

int Tracker::generateNewIncomeId()
{
    return incomes.size();
}