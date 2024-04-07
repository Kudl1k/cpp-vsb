#pragma once

#include <vector>
#include <string>
#include <QDateEdit>

extern std::vector<std::pair<std::string, std::vector<std::string>>> categories;



class Expense {
public:
    Expense(QDate date,std::string category, std::string subcategory,std::string title, double value)
    : date(date),category(category),subcategory(subcategory),title(title),value(value) {};
private:
    QDate date;
    std::string category;
    std::string subcategory;
    std::string title;
    double value;
};