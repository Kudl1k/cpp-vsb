#pragma once

#include <tracker.h>


#include <QMainWindow>
#include <QWidget>
#include <QTabWidget>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QStackedLayout>


class GUI;
class MainDashboardTab;
class IncomesTab;
class ExpansesTab;
class ExpanseLine;


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

    QCheckBox* toggleButton;

    QWidget* createNewExpanse(QVBoxLayout* layout);
    void removeExpanse(ExpanseLine* expanseLine);
    std::vector<ExpanseLine*> expanseLines;
};


class ExpanseLine : public QWidget {
public:
    ExpanseLine(QVBoxLayout* layout, std::function<QWidget*()> createNewExpanse, std::function<void(ExpanseLine*)> removeExpanse);
    QPushButton* getRemoveButton();
    QPushButton* getAddButton();
private:
    QVBoxLayout* layout;
    QPushButton* removeButton;
    QPushButton* addButton;
    int position;
};