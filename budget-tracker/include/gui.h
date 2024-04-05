#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QTabWidget>



class GUI: public QMainWindow {
Q_OBJECT

public:
    GUI(QWidget* parent = nullptr);
private:
    QTabWidget *tabs;
};