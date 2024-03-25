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

Component ui::create_calendar(std::tm *current){
    return Renderer([&]{
        // Get the current date
        time_t now = time(0);
        std::tm tm = *current;

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
        char buff[20];
        strftime(buff, 20, "%Y-%m-%d %H:%M", &tm);
        calendar.push_back(std::vector<Element>{text(buff)});
        std::vector<Element> week = {
            text("Sun")| border,
            text("Mon")| border,
            text("Tue")| border,
            text("Wed")| border,
            text("Thu")| border,
            text("Fri")| border,
            text("Sat")| border
        };
        calendar.push_back(week);

        // Add empty spaces for the first week
        std::vector<Element> calendar_view;
        size_t current_block = 0;
        for (; current_block <= start_day_of_week; current_block++)
        {
            calendar_view.push_back(text("")| border);
        }
        size_t empty_space = current_block;
        for (size_t day = 1; day <= 35; day++, current_block++)
        {
            if (current_block % 7 == 0)
            {
                calendar.push_back(calendar_view);
                calendar_view.clear();
            }
            if (day <= days_in_month)
            {
                calendar_view.push_back(text(std::to_string(day))| border);
            } else {
               calendar_view.push_back(text("")| border);
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



