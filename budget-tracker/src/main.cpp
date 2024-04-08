#include <QApplication>
#include "gui.h"


int main(int argc, char **argv) {
  QApplication a(argc,argv);
  a.setWindowIcon(QIcon("../budget-tracker-ico.png"));

  QFont font("Jetbrains Mono");
  font.setStyleHint(QFont::Monospace);
  QApplication::setFont(font);
    
  GUI g = new GUI(new QWidget);

  g.show();

  return a.exec();
}