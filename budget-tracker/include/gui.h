#pragma once

#include <tracker.h>
#include <goal.h>


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
#include <QScrollArea>
#include <QTableView>
#include <QAbstractItemView>
#include <QStandardItem>
#include <QHeaderView>
#include <QRegularExpressionValidator>
#include <QMenu>
#include <QMenuBar>
#include <QContextMenuEvent>




class GUI;
class MainDashboardTab;
class IncomesTab;
class ExpensesTab;
class ExpenseLine;
class ExpenseTableView;
class IncomeLine;
class IncomeTableView;
class GoalsTab;


class GUI: public QMainWindow {
Q_OBJECT

public:
    GUI(QWidget* parent = nullptr);
private:
    Tracker *tracker;


    QTabWidget* tabs;
    MainDashboardTab* mainDashboardTab;
    IncomesTab* incomesTab;
    ExpensesTab* expensesTab;
    GoalsTab* goalsTab;
};

class MainDashboardTab : public QFrame
{
public:
    MainDashboardTab(Tracker* tracker);
private:
    Tracker* tracker;
};


class ExpensesTab : public QFrame
{
public:
    ExpensesTab(Tracker* tracker);
private:
    Tracker* tracker;

    QCheckBox* toggleButton;

    ExpenseTableView* tableView;

    QScrollArea *scrollarea;

    QWidget* createNewExpense(QVBoxLayout* layout);
    void removeExpense(ExpenseLine* expenseLine);
    std::vector<ExpenseLine*> expenseLines;
};


class ExpenseLine : public QWidget {
public:
    ExpenseLine(QVBoxLayout* layout, std::function<QWidget*()> createNewExpense, std::function<void(ExpenseLine*)> removeExpense);
    QPushButton* getRemoveButton();
    QPushButton* getAddButton();
    QDate getDate();
    QString getCategory();
    QString getSubcategory();
    QString getText();
    QString getValue();
    

private:
    QVBoxLayout* layout;
    QPushButton* removeButton;
    QPushButton* addButton;

    QDateEdit* dateEdit;
    QComboBox* comboBox1;
    QComboBox* comboBox2;
    QLineEdit* textField;
    QLineEdit* valueField;

    int position;
};

class ExpenseTableView : public QTableView
{
public:
    ExpenseTableView(QWidget* parent = nullptr)
        : QTableView(parent)
        , model(new QStandardItemModel(this))
    {
        // Set the column headers
        model->setHorizontalHeaderLabels(QStringList() << "Date" << "Category" << "Subcategory" << "Title" << "Amount");

        // Set the model on the table view
        this->setModel(model);

        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    }

    QStandardItemModel* getModel(){
        return model;
    }

    void addExpense(const QString& date, const QString& category, const QString& subcategory, const QString& amount)
    {
        QList<QStandardItem *> rowItems;
        rowItems.append(new QStandardItem(date));
        rowItems.append(new QStandardItem(category));
        rowItems.append(new QStandardItem(subcategory));
        rowItems.append(new QStandardItem(amount));

    }
protected:
    void contextMenuEvent(QContextMenuEvent* event) override {
        QMenu menu(this);
        QAction* deleteRowAction = menu.addAction("Delete Row");
        connect(deleteRowAction, &QAction::triggered, [this, event]() {
            QModelIndex index = indexAt(event->pos());
            if(index.isValid()) {
                model->removeRow(index.row());
            }
        });
        menu.exec(event->globalPos());
    }

private:
    QStandardItemModel *model;
};


class IncomesTab : public QFrame
{
public:
    IncomesTab(Tracker* tracker);
private:
    Tracker* tracker;

    QCheckBox* toggleButton;

    IncomeTableView* tableView;

    QScrollArea *scrollarea;

    QWidget* createNewIncome(QVBoxLayout* layout);
    void removeIncome(IncomeLine* incomeLine);
    std::vector<IncomeLine*> incomeLines;
};


class IncomeLine : public QWidget {
public:
    IncomeLine(QVBoxLayout* layout, std::function<QWidget*()> createNewIncome, std::function<void(IncomeLine*)> removeIncome);
    QPushButton* getRemoveButton();
    QPushButton* getAddButton();
    QDate getDate();
    QString getCategory();
    QString getSubcategory();
    QString getText();
    QString getValue();

private:
    QVBoxLayout* layout;
    QPushButton* removeButton;
    QPushButton* addButton;

    QDateEdit* dateEdit;
    QComboBox* comboBox1;
    QComboBox* comboBox2;
    QLineEdit* textField;
    QLineEdit* valueField;

    int position;
};

class IncomeTableView : public QTableView
{
public:
    IncomeTableView(QWidget* parent = nullptr)
        : QTableView(parent)
        , model(new QStandardItemModel(this))
    {
        // Set the column headers
        model->setHorizontalHeaderLabels(QStringList() << "Date" << "Category" << "Subcategory" << "Title" << "Amount");

        // Set the model on the table view
        this->setModel(model);

        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    }

    QStandardItemModel* getModel(){
        return model;
    }

    void addIncome(const QString& date, const QString& category, const QString& subcategory, const QString& amount)
    {
        QList<QStandardItem *> rowItems;
        rowItems.append(new QStandardItem(date));
        rowItems.append(new QStandardItem(category));
        rowItems.append(new QStandardItem(subcategory));
        rowItems.append(new QStandardItem(amount));

    }
protected:
    void contextMenuEvent(QContextMenuEvent* event) override {
        QMenu menu(this);
        QAction* deleteRowAction = menu.addAction("Delete Row");
        connect(deleteRowAction, &QAction::triggered, [this, event]() {
            QModelIndex index = indexAt(event->pos());
            if(index.isValid()) {
                model->removeRow(index.row());
            }
        });
        menu.exec(event->globalPos());
    }

private:
    QStandardItemModel *model;
};

class GoalsTab : public QFrame
{
public:
    GoalsTab(Tracker* tracker);
private:
    Tracker* tracker;


    QTableView* tableView;

    std::vector<Goal*> goals;
};


class GoalsTableView : public QTableView
{
public:
    GoalsTableView(QWidget* parent = nullptr)
        : QTableView(parent)
        , model(new QStandardItemModel(this))
    {
        // Set the column headers
        model->setHorizontalHeaderLabels(QStringList() << "Target date" << "Priority" << "Title" << "Goal");

        // Set the model on the table view
        this->setModel(model);

        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    }

    QStandardItemModel* getModel(){
        return model;
    }

    void addIncome(const QString& date, const Priority& priority, const QString& title, const QString& goal)
    {
        QList<QStandardItem *> rowItems;
        rowItems.append(new QStandardItem(date));
        rowItems.append(new QStandardItem(priority));
        rowItems.append(new QStandardItem(title));
        rowItems.append(new QStandardItem(goal));

    }
protected:
    void contextMenuEvent(QContextMenuEvent* event) override {
        QMenu menu(this);
        QAction* deleteRowAction = menu.addAction("Delete Row");
        connect(deleteRowAction, &QAction::triggered, [this, event]() {
            QModelIndex index = indexAt(event->pos());
            if(index.isValid()) {
                model->removeRow(index.row());
            }
        });
        menu.exec(event->globalPos());
    }

private:
    QStandardItemModel *model;
};