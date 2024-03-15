#include "ui.h"




// Component ui::create_welcomescreen(){
//     std::vector<std::string> options = {
//         "Plain",
//         "Simulation"
//     };
// }

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