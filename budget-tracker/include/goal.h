#pragma once

#include <QDateEdit>

enum Priority {
    LOW,
    MEDIUM,
    HIGH,
};


class Goal {
public:
private:
    Priority priority;
    QDate target_date;
    std::string title;
    double goal;
};