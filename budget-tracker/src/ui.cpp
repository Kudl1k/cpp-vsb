#include "ui.h"




Component ui::create_welcomescreen(Component menu){
    return Renderer(menu,[menu]{
        return vbox({
            filler(),
            hbox({
                filler(),
                vbox({
                    filler(),
                    hcenter({
                        text("Budget Tracker") | bold
                    }),
                    vbox({
                        filler(),
                        hcenter({
                            text("Choose your version:") | underlined
                        }),
                        filler(),
                        hcenter({
                            menu->Render()
                        }),
                    }) | flex,
                    filler(),
                    hcenter({
                        text("Created by: KUD0132") | bold
                    })
                }) | flex | border,
                filler()
            }) | flex,
            filler()
        }) | flex;
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