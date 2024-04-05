#include <QApplication>
#include "gui.h"


int main(int argc, char **argv) {
  QApplication a(argc,argv);
  
  GUI g = new GUI(new QWidget);

  g.show();

  return a.exec();
}