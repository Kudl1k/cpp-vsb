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
                    separator(),
                    hcenter({
                        text("Author: KUD0132")
                    })
                })  | size(WIDTH,EQUAL,40) | border,
                filler()
            }) | flex ,            
            filler(),
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