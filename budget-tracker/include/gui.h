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
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QFormLayout>
#include <QMessageBox>



class GUI;
class MainDashboardTab;
class MainGraph;
class IncomesTab;
class ExpensesTab;
class ExpenseLine;
class ExpenseTableView;
class IncomeLine;
class IncomeTableView;
class GoalsTab;
class Next7DaysFilterModel;
class Next14DaysFilterModel;
class Next30DaysFilterModel;


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

class MainGraph : public QChartView
{
public:
    MainGraph(Tracker* tracker);
    QChartView *getChart();
private:
    QChartView *chartView;
};

 
class ExpensesTab : public QFrame
{
public:
    ExpensesTab(Tracker* tracker);
    void removeExpensesFromAddList(ExpenseLine *expenseLine);
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
    QComboBox* getCategory();
    QComboBox* getSubcategory();
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
    ExpenseTableView(QWidget* parent, Tracker *tracker)
        : QTableView(parent)
        , model(new QStandardItemModel(this))
    {
        // Set the column headers
        model->setHorizontalHeaderLabels(QStringList() << "Date" << "Category" << "Subcategory" << "Title" << "Amount");

        // Set the model on the table view
        this->setModel(model);

        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for (auto& record: tracker->getExpenses()) {
            for (size_t i = 0; i < record.second.size(); i++)
            {
                Expense expense = record.second[i];
                addExpense(expense.getDate().toString("yyyy-MM-dd"),QString::fromStdString(expenseCategories[expense.getCategory()].first),QString::fromStdString(expenseCategories[expense.getCategory()].second[expense.getSubcategory()]),QString::fromStdString(std::to_string(expense.getValue())));
            }
        }
        model->sort(0, Qt::AscendingOrder);
    }

    QStandardItemModel* getModel(){
        return model;
    }

    

    void addExpense(const QString& date, const QString& category, const QString& subcategory, const QString& amount)
    {
        QList<QStandardItem *> rowItems;
        QStandardItem *item;

        item = new QStandardItem(date);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        item = new QStandardItem(category);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        item = new QStandardItem(subcategory);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        item = new QStandardItem(amount);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        model->appendRow(rowItems);

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
    void removeIncomeFromAddList(IncomeLine *incomeLine);
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
    QComboBox* getCategory();
    QComboBox* getSubcategory();
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
    IncomeTableView(QWidget* parent,Tracker *tracker)
        : QTableView(parent)
        , model(new QStandardItemModel(this))
    {
        // Set the column headers
        model->setHorizontalHeaderLabels(QStringList() << "Date" << "Category" << "Subcategory" << "Title" << "Amount");
        // Set the model on the table view
        this->setModel(model);
        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for (auto& record: tracker->getIncomes()) {
            for (size_t i = 0; i < record.second.size(); i++)
            {
                Income income = record.second[i];
                addIncome(income.getDate().toString(),QString::fromStdString(incomeCategories[income.getCategory()].first),QString::fromStdString(incomeCategories[income.getCategory()].second[income.getSubcategory()]),QString::fromStdString(std::to_string(income.getValue())));
            }
        }
    }

    QStandardItemModel* getModel(){
        return model;
    }

    // void setFilter(int type){
    //     switch(type) {
    //         case 1:
    //             {
    //                 Next7DaysFilterModel* filterModel = new Next7DaysFilterModel(this);
    //                 filterModel->setSourceModel(model);
    //                 this->setModel(filterModel);
    //                 break;
    //             }
    //         case 2:
    //             {
    //                 Next14DaysFilterModel* filterModel = new Next14DaysFilterModel(this);
    //                 filterModel->setSourceModel(model);
    //                 this->setModel(filterModel);
    //                 break;
    //             }
    //         case 3:
    //             {
    //                 Next30DaysFilterModel* filterModel = new Next30DaysFilterModel(this);
    //                 filterModel->setSourceModel(model);
    //                 this->setModel(filterModel);
    //                 break;
    //             }
    //         case 4:
    //             {
    //                 this->setModel(model);
    //                 break;
    //             }
    //         default:
    //             {
    //                 break;
    //             }
    //     }
    // }

    void addIncome(const QString& date, const QString& category, const QString& subcategory, const QString& amount)
    {
        QList<QStandardItem *> rowItems;
        QStandardItem *item;

        item = new QStandardItem(date);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        item = new QStandardItem(category);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        item = new QStandardItem(subcategory);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        item = new QStandardItem(amount);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        rowItems.append(item);

        model->appendRow(rowItems);

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


class Next7DaysFilterModel : public QSortFilterProxyModel {
public:
    Next7DaysFilterModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        QDate date = index.data().toDate();

        QDate today = QDate::currentDate();
        QDate nextWeek = today.addDays(7);

        return date >= today && date <= nextWeek;
    }
};

class Next14DaysFilterModel : public QSortFilterProxyModel {
public:
    Next14DaysFilterModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        QDate date = index.data().toDate();

        QDate today = QDate::currentDate();
        QDate nextWeek = today.addDays(14);

        return date >= today && date <= nextWeek;
    }
};

class Next30DaysFilterModel : public QSortFilterProxyModel {
public:
    Next30DaysFilterModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        QDate date = index.data().toDate();

        QDate today = QDate::currentDate();
        QDate nextWeek = today.addMonths(1);

        return date >= today && date <= nextWeek;
    }
};