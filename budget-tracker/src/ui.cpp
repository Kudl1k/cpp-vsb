#include "ui.h"




Component ui::create_welcomescreen(Component comp, std::vector<Component> options){
    return Renderer(comp,[options]{
        return vbox({
            filler(),
            hbox({
                filler(),
                vbox({
                    hcenter({text("Budget Tracker") | bold | underlined}),
                    separator(),
                    vcenter({
                        hcenter({
                            vbox({
                                hcenter({options[0]->Render()}),
                                hcenter({options[1]->Render()})
                            }) | flex
                        }),
                    
                    })| flex,
                    filler(),
                    hcenter({options[2]->Render()}),
                    separator(),
                    hcenter({
                        text("Author: KUD0132")
                    })
                })  | size(WIDTH,EQUAL,60) | border,
                filler()
            }) | flex ,            
            filler(),
        }) | flex;
    });
}

Component ui::create_calendar(time_t &result, time_t current){
    return Renderer([&]{
        // Get the current date
        std::tm tm = *std::localtime(&current);

        // Get the day of the week for the first day of the month
        struct tm first_day = {0};
        first_day.tm_year = tm.tm_year; // Years since 1900
        first_day.tm_mon = tm.tm_mon; // Month index is 0-based, so subtract 1
        first_day.tm_mday = 1;
        mktime(&first_day); // Normalize the struct tm
        int start_day_of_week = tm.tm_wday;

        // Get the number of days in the month
        int days_in_month = 31; // Default to a maximum value
        if (tm.tm_mon == 3 || tm.tm_mon == 5 || tm.tm_mon == 8 || tm.tm_mon == 10) {
            days_in_month = 30;
        } else if (tm.tm_mon == 1) {
            if ((tm.tm_year % 4 == 0 && tm.tm_year % 100 != 0) || (tm.tm_year % 400 == 0)) {
                days_in_month = 29;
            } else {
                days_in_month = 28;
            }
        }

        std::vector<std::vector<Element>> calendar;
        std::vector<Element> week = {
            text("Mon")| border,
            text("Tue")| border,
            text("Wed")| border,
            text("Thu")| border,
            text("Fri")| border,
            text("Sat")| border,
            text("Sun")| border,
        };
        calendar.push_back(week);

        // Add empty spaces for the first week
        std::vector<Element> first_week(7, text("    ")|border);
        for (int i = 0; i < start_day_of_week; ++i) {
            first_week[i] = text("    ")|border;
        }

        // Print the calendar
        for (int day = 1; day <= days_in_month; ++day) {
            first_week[(start_day_of_week + day - 1) % 7] = text(std::to_string(day))|border;
            if ((start_day_of_week + day) % 7 == 0 || day == days_in_month) {
                calendar.push_back(first_week);
                first_week = std::vector<Element>(7, text("    ")|border);
            }
        }

        return hbox({
            gridbox(calendar)
        }) | flex | border;
    });
}


Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 8),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}



