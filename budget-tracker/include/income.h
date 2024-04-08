#pragma once

#include <vector>
#include <string>

#include <QDateEdit>


extern std::vector<std::pair<std::string, std::vector<std::string>>> incomeCategories;


class Income {
public:
    Income(QDate date, std::string category, std::string subcategory, std::string title, double value)
    : date(date.toString("yyyy-MM-dd").toStdString()), category(category), subcategory(subcategory), title(title), value(value) {};
    QDate getDate() const { return QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd"); }
    std::string getCategory() const { return category; }
    std::string getSubcategory() const { return subcategory; }
    std::string getTitle() const { return title; }
    double getValue() const { return value; }
private:
    std::string date;
    std::string category;
    std::string subcategory;
    std::string title;
    double value;
};