#pragma once

#include <tracker.h>


#include <QMainWindow>
#include <QWidget>
#include <QTabWidget>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>

class GUI;
class MainDashboardTab;
class IncomesTab;
class ExpansesTab;



class GUI: public QMainWindow {
Q_OBJECT

public:
    GUI(QWidget* parent = nullptr);
private:
    Tracker *tracker;


    QTabWidget* tabs;
    MainDashboardTab* mainDashboardTab;
    IncomesTab* incomesTab;
    ExpansesTab* expansesTab;
};

class MainDashboardTab : public QFrame
{
public:
    MainDashboardTab(Tracker* tracker);
private:
    Tracker* tracker;
};

class IncomesTab : public QFrame
{
public:
    IncomesTab(Tracker* tracker);
private:
    Tracker* tracker;
};

class ExpansesTab : public QFrame
{
public:
    ExpansesTab(Tracker* tracker);
private:
    Tracker* tracker;
};