#pragma once

#include <vector>
#include <string>
#include <QDateEdit>

extern std::vector<std::pair<std::string, std::vector<std::string>>> expenseCategories;



class Expense {
public:
    Expense(QDate date, int category, int subcategory, std::string title, double value)
        : date(date.toString("yyyy-MM-dd").toStdString()), category(category), subcategory(subcategory), title(title), value(value) {};
    QDate getDate() const { 
        return QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd"); 
    }
    int getCategory() const { return category; }
    int getSubcategory() const { return subcategory; }
    std::string getTitle() const { return title; }
    double getValue() const { return value; }


private:
    std::string date;
    int category;
    int subcategory;
    std::string title;
    double value;
};