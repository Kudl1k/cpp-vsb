#pragma once

#include <QDateEdit>

enum Priority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
};


class Goal {
public:
private:
    Priority priority;
    std::string target_date;
    std::string title;
    double goal;
};