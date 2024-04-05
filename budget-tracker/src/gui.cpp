#include "gui.h"


GUI::GUI(QWidget* parent) : QMainWindow(parent)
{
    this->setWindowTitle("Budget Tracker - KUD0132");
    this->setMinimumSize(640,480);
    this->move(100,100);

    tabs = new QTabWidget();
    tabs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);


}