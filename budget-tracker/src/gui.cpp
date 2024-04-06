#include "gui.h"


GUI::GUI(QWidget* parent) : QMainWindow(parent)
{
    tracker = new Tracker();


    this->setWindowTitle("Budget Tracker - KUD0132");
    this->setMinimumSize(640,480);
    this->move(100,100);

    tabs = new QTabWidget();
    tabs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);


    mainDashboardTab = new MainDashboardTab(tracker);
    incomesTab = new IncomesTab(tracker);
    expansesTab = new ExpansesTab(tracker);
    
    
    tabs->addTab(mainDashboardTab, QString("Main Dashboard"));
    tabs->addTab(incomesTab, QString("Incomes"));
    tabs->addTab(expansesTab, QString("Expanses"));


    setCentralWidget(tabs);

}

MainDashboardTab::MainDashboardTab(Tracker *tracker) : tracker(tracker)
{
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(new QLabel(QString("Main Dashboard")),0,0);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout);
}

IncomesTab::IncomesTab(Tracker *tracker)
{
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(new QLabel(QString("Incomes")),0,0);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout);
}

ExpansesTab::ExpansesTab(Tracker *tracker)
{
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(new QLabel(QString("Expanses")),0,0);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout);
}
