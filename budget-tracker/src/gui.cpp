#include "gui.h"


GUI::GUI(QWidget* parent) : QMainWindow(parent)
{
    tracker = new Tracker();

    this->setWindowTitle("Budget Tracker - KUD0132");
    this->setMinimumSize(1280,720);
    this->move(100,100);

    tabs = new QTabWidget();
    tabs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);


    //TODO: create menu
    QMenuBar *menu = this->menuBar();
    QMenu *navigation = menu->addMenu("Navigation");
    navigation->addAction("Main Dashboard");


    mainDashboardTab = new MainDashboardTab(tracker);
    incomesTab = new IncomesTab(tracker);
    expensesTab = new ExpensesTab(tracker);
    goalsTab = new GoalsTab(tracker);
    
    tabs->addTab(mainDashboardTab, QString("Main Dashboard"));
    tabs->addTab(incomesTab, QString("Incomes"));
    tabs->addTab(expensesTab, QString("Expenses"));
    tabs->addTab(goalsTab, QString("Goals"));


    setCentralWidget(tabs);

}

MainDashboardTab::MainDashboardTab(Tracker *tracker) : tracker(tracker)
{
    //TODO: Create ui
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(new QLabel(QString("Main Dashboard")),0,0);

    MainGraph* maingraph = new MainGraph(tracker);

    layout->addWidget(maingraph->getChart());

    this->setLayout(layout);
}

MainGraph::MainGraph(Tracker* tracker)
{
    QPieSeries *series = new QPieSeries();
    series->append("Category 1", 30);
    series->append("Category 2", 50);
    series->append("Category 3", 20);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pie Chart");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

QChartView *MainGraph::getChart()
{
    return chartView;
}

ExpensesTab::ExpensesTab(Tracker *tracker)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    //* Header section
    QHBoxLayout* header = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("New");
    header->addWidget(editButton,0);
    QPushButton* addExpenses = new QPushButton("Add expenses");
    addExpenses->hide();
    header->addWidget(addExpenses,0);
    header->addStretch(1);
    QLabel* filterLabel = new QLabel("Display:");
    header->addWidget(filterLabel);
    QComboBox* filterComboBox = new QComboBox();
    filterComboBox->addItem("All");
    filterComboBox->addItem("Next 7 days");
    filterComboBox->addItem("Next 14 days");
    filterComboBox->addItem("Next month");
    header->addWidget(filterComboBox);

    //* Add/Edit section
    toggleButton = new QCheckBox("Toggle adding multiple expenses");
    toggleButton->hide();

    QVBoxLayout* expenseLinesLayout = new QVBoxLayout();
    scrollarea = new QScrollArea();
    scrollarea->hide();
    scrollarea->setFixedHeight(75);
    QWidget *scrollWidget = new QWidget();
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    expenseLinesLayout->setAlignment(Qt::AlignTop);
    scrollWidget->setLayout(expenseLinesLayout);
    scrollarea->setWidgetResizable(true);
    scrollarea->setWidget(scrollWidget);

    tableView = new ExpenseTableView(this);

    mainLayout->addLayout(header);
    mainLayout->addWidget(toggleButton);
    mainLayout->addWidget(scrollarea);
    mainLayout->addWidget(tableView);

    this->setLayout(mainLayout);

    connect(toggleButton, &QCheckBox::stateChanged, [this](int state) {
        if (state == Qt::Checked) {
            for (auto& expenseLine : this->expenseLines) {
                expenseLine->show();
                expenseLine->getRemoveButton()->show();
                expenseLine->getAddButton()->show();
                scrollarea->setFixedHeight(200);

            }
        } else {
            for (auto& expenseLine : this->expenseLines) {
                expenseLine->hide();
                expenseLine->getRemoveButton()->hide();
                expenseLine->getAddButton()->hide();
                scrollarea->setFixedHeight(75);
            }
            if (!this->expenseLines.empty()) {
                this->expenseLines.front()->show();
            }
        }
    });

    connect(editButton, &QPushButton::clicked, [this,expenseLinesLayout,addExpenses]() {
        if (this->expenseLines.size() == 0)
        {
            this->createNewExpense(expenseLinesLayout);
        }
        
        if (toggleButton->isVisible())
        {
            addExpenses->hide();
            toggleButton->hide();
            scrollarea->hide(); 
            for (auto& expenseLine : this->expenseLines) {
                expenseLine->hide();
            }
        } else {
            addExpenses->show();
            toggleButton->show();
            scrollarea->show();
            for (auto& expenseLine : this->expenseLines) {
                expenseLine->show();
            }
        }
    });


    connect(addExpenses, &QPushButton::clicked, [this,tracker]() {
        for (auto& expenseLine : this->expenseLines) {
            QDate date = expenseLine->getDate();
            QString category = expenseLine->getCategory();
            QString subcategory = expenseLine->getSubcategory();
            QString text = expenseLine->getText();
            QString value = expenseLine->getValue();

            QList<QStandardItem*> items;
            items.append(new QStandardItem(date.toString()));
            items.append(new QStandardItem(category));
            items.append(new QStandardItem(subcategory));
            items.append(new QStandardItem(text));
            items.append(new QStandardItem(value));

            std::pair<bool,std::string> added = tracker->addExpense(
                date,
                category.toStdString(),
                subcategory.toStdString(),
                text.toStdString(),
                value.toDouble()
            );
            if (added.first)
            {
                tableView->getModel()->appendRow(items);
                tableView->hideColumn(tableView->getModel()->columnCount());
                this->removeExpense(expenseLine);
            }
        }
        std::cout << tracker->getExpenses().size() << std::endl;
    });
}

QWidget * ExpensesTab::createNewExpense(QVBoxLayout* layout){
    ExpenseLine* newExpenseLine = new ExpenseLine(layout, [this, layout](){ return this->createNewExpense(layout); }, [this](ExpenseLine* expenseLine){ this->removeExpense(expenseLine); });
    this->expenseLines.push_back(newExpenseLine);
    layout->addWidget(newExpenseLine);    
    if (this->expenseLines.size() == 1) {
        if (!this->toggleButton->isChecked())
        {
            this->expenseLines.front()->getRemoveButton()->hide();
            this->expenseLines.front()->getAddButton()->hide();
        } else {
            newExpenseLine->getRemoveButton()->setDisabled(true);
        }
    } else {
        if (!this->toggleButton->isChecked())
        {
            this->expenseLines.front()->getRemoveButton()->hide();
            this->expenseLines.front()->getAddButton()->hide();
        } else {
            this->expenseLines.front()->getRemoveButton()->setDisabled(false);
            this->expenseLines.front()->getAddButton()->show();
        }
    }
    return newExpenseLine;
}

void ExpensesTab::removeExpense(ExpenseLine *expenseLine){
    auto it = std::find(expenseLines.begin(), expenseLines.end(), expenseLine);
    if (it != expenseLines.end()) {
        expenseLines.erase(it);
        expenseLine->deleteLater();
    }
    if (expenseLines.empty()) {
        toggleButton->hide();
        scrollarea->hide();
    } else if (expenseLines.size() == 1) {
        expenseLines.front()->getRemoveButton()->setDisabled(true);
    }
}

ExpenseLine::ExpenseLine(QVBoxLayout* layout, std::function<QWidget*()> createNewExpense, std::function<void(ExpenseLine*)> removeExpense) : layout(layout), position(position) {
    QGridLayout* lineLayout = new QGridLayout();

    dateEdit = new QDateEdit();
    comboBox1 = new QComboBox();
    comboBox1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    comboBox2 = new QComboBox();
    comboBox2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    textField = new QLineEdit();
    textField->setPlaceholderText("New thing");


    valueField = new QLineEdit();
    valueField->setPlaceholderText("Enter value");
    QValidator *validator = new QDoubleValidator(this);
    valueField->setValidator(validator);

    textField->setDisabled(true);
    valueField->setDisabled(true);
    textField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);


    removeButton = new QPushButton("-");
    removeButton->setStyleSheet("color: red");
    removeButton->setToolTip("Remove");
    removeButton->setFixedWidth(30);
    addButton = new QPushButton("+");
    addButton->setStyleSheet("color: green");
    addButton->setToolTip("Add next expense");
    addButton->setFixedWidth(30);


    dateEdit->setDate(QDate::currentDate());

    for (size_t i = 0; i < expenseCategories.size(); i++)
    {
        comboBox1->addItem(QString::fromStdString(expenseCategories[i].first));
    }
    comboBox1->setCurrentIndex(0);

    lineLayout->addWidget(dateEdit, 0, 0);
    lineLayout->addWidget(comboBox1, 0, 1);
    lineLayout->addWidget(comboBox2, 0, 2);
    lineLayout->addWidget(textField, 0, 3);
    lineLayout->addWidget(valueField, 0, 4);
    lineLayout->addWidget(removeButton,0,5);
    lineLayout->addWidget(addButton, 0, 6);

    this->setLayout(lineLayout);

    connect(comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index){
        comboBox2->clear();
        if(index >= 0 && index < expenseCategories.size()) {
            for(const auto& subcategory : expenseCategories[index].second) {
                comboBox2->addItem(QString::fromStdString(subcategory));
            }
        }
        comboBox2->setCurrentIndex(0);
        if (!comboBox2->currentText().isEmpty())
        {
            textField->setDisabled(false);
            valueField->setDisabled(false);
        } else {
            textField->setDisabled(true);
            valueField->setDisabled(true);
        }
        
    });
    connect(addButton, &QPushButton::clicked, [createNewExpense](){
        createNewExpense();
    });
    connect(removeButton, &QPushButton::clicked, [this,layout,removeExpense]() {
        layout->removeWidget(this);
        removeExpense(this);
        delete this;
    });
}

QPushButton* ExpenseLine::getRemoveButton() {
    return removeButton;
}

QPushButton * ExpenseLine::getAddButton()
{
    return addButton;
}


QDate ExpenseLine::getDate() {
    return dateEdit->date();
}

QString ExpenseLine::getCategory() {
    return comboBox1->currentText();
}

QString ExpenseLine::getSubcategory() {
    return comboBox2->currentText();
}

QString ExpenseLine::getText() {
    return textField->text();
}

QString ExpenseLine::getValue() {
    return valueField->text();
}

IncomesTab::IncomesTab(Tracker *tracker)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QHBoxLayout* header = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("New");
    QPushButton* addIncomes = new QPushButton("Add incomes");
    addIncomes->hide();
    QLabel* filterLabel = new QLabel("Display:");
    QComboBox* filterComboBox = new QComboBox();
    filterComboBox->addItem("All");
    filterComboBox->addItem("Next 7 days");
    filterComboBox->addItem("Next 14 days");
    filterComboBox->addItem("Next month");

    header->addWidget(editButton,0);
    header->addWidget(addIncomes,0);
    header->addStretch(1);
    header->addWidget(filterLabel);
    header->addWidget(filterComboBox);


    toggleButton = new QCheckBox("Toggle adding multiple incomes");
    toggleButton->hide();

    QVBoxLayout* incomeLinesLayout = new QVBoxLayout();
    incomeLinesLayout->setAlignment(Qt::AlignTop);
    QWidget *scrollWidget = new QWidget();
    scrollWidget->setLayout(incomeLinesLayout);
    scrollarea = new QScrollArea();
    scrollarea->hide();
    scrollarea->setFixedHeight(75);
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollarea->setWidgetResizable(true);
    scrollarea->setWidget(scrollWidget);

    tableView = new IncomeTableView(this);

    mainLayout->addLayout(header);
    mainLayout->addWidget(toggleButton);
    mainLayout->addWidget(scrollarea);
    mainLayout->addWidget(tableView);

    this->setLayout(mainLayout);

    connect(toggleButton, &QCheckBox::stateChanged, [this, incomeLinesLayout](int state) {
        if (state == Qt::Checked) {
            for (auto& incomeLine : this->incomeLines) {
                incomeLine->show();
                incomeLine->getRemoveButton()->show();
                incomeLine->getAddButton()->show();
                scrollarea->setFixedHeight(200);
            }
        } else {
            for (auto& incomeLine : this->incomeLines) {
                incomeLine->hide();
                incomeLine->getRemoveButton()->hide();
                incomeLine->getAddButton()->hide();
                scrollarea->setFixedHeight(75);
            }
            if (!this->incomeLines.empty()) {
                this->incomeLines.front()->show();
            }
        }
    });

    connect(editButton, &QPushButton::clicked, [this,incomeLinesLayout,addIncomes]() {
        if (this->incomeLines.size() == 0)
        {
            this->createNewIncome(incomeLinesLayout);
        }

        if (toggleButton->isVisible())
        {
            addIncomes->hide();
            toggleButton->hide();
            scrollarea->hide(); 
            for (auto& incomeLine : this->incomeLines) {
                incomeLine->hide();
            }
        } else {
            addIncomes->show();
            toggleButton->show();
            scrollarea->show();
            for (auto& incomeLine : this->incomeLines) {
                incomeLine->show();
            }
        }
    });

    connect(addIncomes, &QPushButton::clicked, [this,tracker,addIncomes]() {
        for (auto& incomeLine : this->incomeLines) {
            QDate date = incomeLine->getDate();
            QString category = incomeLine->getCategory();
            QString subcategory = incomeLine->getSubcategory();
            QString text = incomeLine->getText();
            QString value = incomeLine->getValue();

            QList<QStandardItem*> items;
            items.append(new QStandardItem(date.toString()));
            items.append(new QStandardItem(category));
            items.append(new QStandardItem(subcategory));
            items.append(new QStandardItem(text));
            items.append(new QStandardItem(value));

            std::pair<bool,std::string> added = tracker->addIncome(
                date,
                category.toStdString(),
                subcategory.toStdString(),
                text.toStdString(),
                value.toDouble()
            );
            if (added.first)
            {
                tableView->getModel()->appendRow(items);
                tableView->hideColumn(tableView->getModel()->columnCount());
                this->removeIncome(incomeLine);
            }
        }
        if (this->incomeLines.size() == 0)
        {
            addIncomes->hide();
            toggleButton->hide();
            scrollarea->hide(); 
        }
        
        std::cout << tracker->getIncomes().size() << std::endl;
    });

    
}

QWidget * IncomesTab::createNewIncome(QVBoxLayout* layout){
    IncomeLine* newIncomeLine = new IncomeLine(layout, [this, layout](){ return this->createNewIncome(layout); }, [this](IncomeLine* incomeLine){ this->removeIncome(incomeLine); });
    this->incomeLines.push_back(newIncomeLine);
    layout->addWidget(newIncomeLine);
    std::cout << "Number of lines: " << this->incomeLines.size() << std::endl;
    
    if (this->incomeLines.size() == 1) {
        if (!this->toggleButton->isChecked())
        {
            this->incomeLines.front()->getRemoveButton()->hide();
            this->incomeLines.front()->getAddButton()->hide();
        } else {
            newIncomeLine->getRemoveButton()->setDisabled(true);
        }
    } else {
        if (!this->toggleButton->isChecked())
        {
            this->incomeLines.front()->getRemoveButton()->hide();
            this->incomeLines.front()->getAddButton()->hide();
        } else {
            this->incomeLines.front()->getRemoveButton()->setDisabled(false);
            this->incomeLines.front()->getAddButton()->show();
        }
    }
    return newIncomeLine;
}

void IncomesTab::removeIncome(IncomeLine *incomeLine){
    auto it = std::find(incomeLines.begin(), incomeLines.end(), incomeLine);
    if (it != incomeLines.end()) {
        incomeLines.erase(it);
        incomeLine->deleteLater();
    }
    if (incomeLines.empty()) {
        toggleButton->hide();
        scrollarea->hide();
    } else if (incomeLines.size() == 1) {
        incomeLines.front()->getRemoveButton()->setDisabled(true);
    }
}

IncomeLine::IncomeLine(QVBoxLayout* layout, std::function<QWidget*()> createNewIncome, std::function<void(IncomeLine*)> removeIncome) : layout(layout), position(position) {
    QGridLayout* lineLayout = new QGridLayout();

    dateEdit = new QDateEdit();
    dateEdit->setDate(QDate::currentDate());

    comboBox1 = new QComboBox();
    comboBox1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    for (size_t i = 0; i < incomeCategories.size(); i++)
    {
        comboBox1->addItem(QString::fromStdString(incomeCategories[i].first));
    }
    comboBox1->setCurrentIndex(0);

    comboBox2 = new QComboBox();
    comboBox2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    textField = new QLineEdit();
    textField->setPlaceholderText("New thing");
    textField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    textField->setDisabled(true);

    valueField = new QLineEdit();
    valueField->setPlaceholderText("Enter value");
    QValidator *validator = new QDoubleValidator(this);
    valueField->setValidator(validator);
    valueField->setDisabled(true);

    removeButton = new QPushButton("-");
    removeButton->setStyleSheet("color: red");
    removeButton->setToolTip("Remove");
    removeButton->setFixedWidth(30);

    addButton = new QPushButton("+");
    addButton->setStyleSheet("color: green");
    addButton->setToolTip("Add next income");
    addButton->setFixedWidth(30);

    lineLayout->addWidget(dateEdit, 0, 0);
    lineLayout->addWidget(comboBox1, 0, 1);
    lineLayout->addWidget(comboBox2, 0, 2);
    lineLayout->addWidget(textField, 0, 3);
    lineLayout->addWidget(valueField, 0, 4);
    lineLayout->addWidget(removeButton,0,5);
    lineLayout->addWidget(addButton, 0, 6);

    this->setLayout(lineLayout);
    
    connect(comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index){
        comboBox2->clear();
        if(index >= 0 && index < incomeCategories.size()) {
            for(const auto& subcategory : incomeCategories[index].second) {
                comboBox2->addItem(QString::fromStdString(subcategory));
            }
        }
        comboBox2->setCurrentIndex(0);
        if (!comboBox2->currentText().isEmpty())
        {
            textField->setDisabled(false);
            valueField->setDisabled(false);
        } else {
            textField->setDisabled(true);
            valueField->setDisabled(true);
        }
        
    });
    connect(addButton, &QPushButton::clicked, [createNewIncome](){
        createNewIncome();
    });
    connect(removeButton, &QPushButton::clicked, [this,layout,removeIncome]() {
        layout->removeWidget(this);
        removeIncome(this);
        delete this;
    });
}

QPushButton* IncomeLine::getRemoveButton() {
    return removeButton;
}

QPushButton * IncomeLine::getAddButton()
{
    return addButton;
}


QDate IncomeLine::getDate() {
    return dateEdit->date();
}

QString IncomeLine::getCategory() {
    return comboBox1->currentText();
}

QString IncomeLine::getSubcategory() {
    return comboBox2->currentText();
}

QString IncomeLine::getText() {
    return textField->text();
}

QString IncomeLine::getValue() {
    return valueField->text();
}

GoalsTab::GoalsTab(Tracker * tracker) : tracker(tracker)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QHBoxLayout* header = new QHBoxLayout();

    QPushButton* editButton = new QPushButton("New");
    QPushButton* addGoalButton = new QPushButton("Add Goal");
    addGoalButton->hide();

    QLabel* filterLabel = new QLabel("Display:");

    QComboBox* filterComboBox = new QComboBox();
    filterComboBox->addItem("All");
    filterComboBox->addItem("Next 7 days");
    filterComboBox->addItem("Next 14 days");
    filterComboBox->addItem("Next month");

    header->addWidget(editButton,0);
    header->addWidget(addGoalButton,0);
    header->addStretch(1);
    header->addWidget(filterLabel);
    header->addWidget(filterComboBox);

    QVBoxLayout* addGoal = new QVBoxLayout();

    QVBoxLayout* bottomWidget = new QVBoxLayout();
    tableView = new GoalsTableView(this);

    mainLayout->addLayout(header);
    
    QHBoxLayout* addSection = new QHBoxLayout();


    QDateEdit* datePicker = new QDateEdit();
    QComboBox* priorityComboBox = new QComboBox();
    priorityComboBox->addItem("Low");
    priorityComboBox->addItem("Medium");
    priorityComboBox->addItem("High");
    QLineEdit* titleTextField = new QLineEdit();
    QLineEdit* valueTextField = new QLineEdit();

    addSection->addWidget(datePicker);
    addSection->addWidget(priorityComboBox);
    addSection->addWidget(titleTextField);
    addSection->addWidget(valueTextField);

    addGoal->addLayout(addSection);
    addGoal->addLayout(bottomWidget);
    mainLayout->addLayout(addGoal);




    mainLayout->addWidget(tableView);



    this->setLayout(mainLayout);
}