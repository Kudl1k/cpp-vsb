#include "gui.h"


GUI::GUI(QWidget* parent) : QMainWindow(parent)
{
    this->tracker = new Tracker(nullptr);
    this->setWindowTitle("Budget Tracker - KUD0132");
    this->setMinimumSize(1280,720);
    this->move(100,100);

    tabs = new QTabWidget();
    tabs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    welcomePage = new WelcomePage(this);
    setupProfilePage = new SetupProfilePage(this->parentWidget(),this);
    setCentralWidget(welcomePage);

    connect(welcomePage->continueButton, &QPushButton::clicked, this, [this](){
        this->tracker->loadFromFile();
        this->switchToMainDashboard();
    });
    connect(welcomePage->newProfileButton, &QPushButton::clicked, this, &GUI::switchToSetupProfile);
}

void GUI::closeEvent(QCloseEvent *event)
{
    this->tracker->saveToFile();
    event->accept();
    QApplication::quit();
}

void GUI::switchToMainDashboard() {
    if (this->tracker->getUser() == nullptr)
    {
        QMessageBox::warning(this,"Warning","Please setup your profile first.");
        this->tracker = new Tracker(nullptr);
        return;
    }
    std::cout << "Switching to main dashboard" << std::endl;
    mainDashboardTab = new MainDashboardTab(tracker);
    incomesTab = new IncomesTab(tracker,mainDashboardTab);
    expensesTab = new ExpensesTab(tracker, mainDashboardTab);
    goalsTab = new GoalsTab(tracker);
    tabs->addTab(mainDashboardTab, QString("Main Dashboard"));
    tabs->addTab(incomesTab, QString("Incomes"));
    tabs->addTab(expensesTab, QString("Expenses"));
    tabs->addTab(goalsTab, QString("Goals"));
    setCentralWidget(tabs);


    QMenuBar *menu = this->menuBar();

    QMenu *settings = menu->addMenu("Profile");
    QAction *saveProfileAction = settings->addAction("Save profile");
    saveProfileAction->setShortcut(QKeySequence::Save);
    connect(saveProfileAction, &QAction::triggered, this, [this](){
        this->tracker->saveToFile();
        QMessageBox::information(this,"Information","Profile saved.");
    });

    QMenu *viewmode = menu->addMenu("View mode");
    QActionGroup *actionGroup = new QActionGroup(this);

    QAction *allAction = viewmode->addAction("Current month");
    allAction->setCheckable(true);
    allAction->setActionGroup(actionGroup);
    connect(allAction, &QAction::triggered, this, [this](){
        this->tracker->setViewMode(0);
        this->mainDashboardTab->refreshUI();
    });

    QAction *next7days = viewmode->addAction("This week");
    next7days->setCheckable(true);
    next7days->setActionGroup(actionGroup);
    connect(next7days, &QAction::triggered, this, [this](){
        this->tracker->setViewMode(1);
        this->mainDashboardTab->refreshUI();
    });

    QAction *next14days = viewmode->addAction("Next week");
    next14days->setCheckable(true);
    next14days->setActionGroup(actionGroup);
    connect(next14days, &QAction::triggered, this, [this](){
        this->tracker->setViewMode(2);
        this->mainDashboardTab->refreshUI();
    });

    QAction *nextMonth = viewmode->addAction("Next month");
    nextMonth->setCheckable(true);
    nextMonth->setActionGroup(actionGroup);
    connect(nextMonth, &QAction::triggered, this, [this](){
        this->tracker->setViewMode(3);
        this->mainDashboardTab->refreshUI();
    });
}

void GUI::createTracker(User* user)
{
    std::cout << "Assigning tracker to GUI..." << std::endl;
    this->tracker = new Tracker(user);
    switchToMainDashboard();
    std::cout << "Switched to main dashboard" << std::endl;
}

void GUI::switchToSetupProfile()
{
    setCentralWidget(setupProfilePage);
}


WelcomePage::WelcomePage(QWidget *parent)
{
    QLabel* title = new QLabel("Budget Tracker");
    QFont font = title->font();
    font.setPointSize(32);
    font.setBold(true);
    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);
    newProfileButton = new QPushButton("New profile", this);
    continueButton = new QPushButton("Load profile", this);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addSpacing(50);
    layout->addWidget(newProfileButton);
    layout->addWidget(continueButton);
    layout->setAlignment(Qt::AlignCenter);
    this->setLayout(layout);
}

SetupProfilePage::SetupProfilePage(QWidget *parent, GUI *gui)
{
    this->gui = gui;
    QLabel* title = new QLabel("Setup your profile");
    QFont font = title->font();
    font.setPointSize(32);
    font.setBold(true);
    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);
    QLabel* nameLabel = new QLabel("Name:");
    QLineEdit* nameInput = new QLineEdit();
    QLabel* expensePercentage = new QLabel("Expense percentage:");
    QSpinBox* expensePercentageInput = new QSpinBox();
    expensePercentageInput->setRange(0, 100);
    expensePercentageInput->setValue(80);



    QLabel* savingsPercentage = new QLabel("Savings percentage:");
    QSpinBox* savingsPercentageInput = new QSpinBox();
    savingsPercentageInput->setRange(0, 100);
    savingsPercentageInput->setValue(20);



    QPushButton* submitButton = new QPushButton("Submit");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addSpacing(50);
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow(nameLabel, nameInput);
    formLayout->addRow(expensePercentage, expensePercentageInput);
    formLayout->addRow(savingsPercentage, savingsPercentageInput);
    

    QWidget* formWidget = new QWidget();
    formWidget->setLayout(formLayout);

    layout->addWidget(formWidget, 0, Qt::AlignCenter);
    layout->addWidget(submitButton,0, Qt::AlignCenter);
    layout->setAlignment(Qt::AlignCenter);
    this->setLayout(layout);

    connect(submitButton, &QPushButton::clicked, [this, nameInput, expensePercentageInput, savingsPercentageInput]() {
        
        std::string name = nameInput->text().toStdString();
        int expensePercentage = expensePercentageInput->value();
        int savingsPercentage = savingsPercentageInput->value();
        if (name == "")
        {
            QMessageBox::warning(this,"Warning","Please fill in the name field.");
        } else {
            this->gui->createTracker(new User(name, expensePercentage, savingsPercentage));
        }
    });

    connect(expensePercentageInput, QOverload<int>::of(&QSpinBox::valueChanged), [savingsPercentageInput](int newValue) {
    int otherValue = 100 - newValue;
        // Block signals to avoid infinite loop
        savingsPercentageInput->blockSignals(true);
        savingsPercentageInput->setValue(otherValue);
        savingsPercentageInput->blockSignals(false);
    });

    connect(savingsPercentageInput, QOverload<int>::of(&QSpinBox::valueChanged), [expensePercentageInput](int newValue) {
        int otherValue = 100 - newValue;
        // Block signals to avoid infinite loop
        expensePercentageInput->blockSignals(true);
        expensePercentageInput->setValue(otherValue);
        expensePercentageInput->blockSignals(false);
    });
}

MainDashboardTab::MainDashboardTab(Tracker *tracker) : tracker(tracker)
{
    //TODO: Create ui
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* header = new QHBoxLayout();

    std::string helloUser = "Hello " + tracker->getUser()->getName();
    QLabel *label = new QLabel(QString::fromStdString(helloUser));
    QFont font = label->font();
    font.setPointSize(21);
    font.setBold(true);
    label->setFont(font);

    QLabel *descriptioQuickActions = new QLabel("Quick actions: ");
    QPushButton* fastAddIncome = new QPushButton("Add income");
    QPushButton* fastAddExpanse = new QPushButton("Add expense");
    QPushButton* fastAddGoal = new QPushButton("Add goal");

    mainInfo = new MainInfo(tracker);


    header->addWidget(label);
    header->addStretch(2);
    header->addWidget(mainInfo);
    header->addStretch(2);
    header->addWidget(descriptioQuickActions);
    header->addWidget(fastAddIncome);
    header->addWidget(fastAddExpanse);
    header->addWidget(fastAddGoal);
    
    QHBoxLayout *body = new QHBoxLayout();

    QVBoxLayout *leftBody = new QVBoxLayout();
    QVBoxLayout *middleBody = new QVBoxLayout();
    QVBoxLayout *rightBody = new QVBoxLayout();


    incomesGraph = new IncomesGraph(tracker);
    expensesGraph = new ExpensesGraph(tracker);
    MainGraph* maingraph3 = new MainGraph(tracker);
    



    leftBody->addWidget(incomesGraph->getChart());
    middleBody->addWidget(expensesGraph->getChart());
    //rightBody->addWidget(maingraph3->getChart());


    QWidget *widget1 = new QWidget();
    widget1->setLayout(leftBody);
    QWidget *widget2 = new QWidget();
    widget2->setLayout(middleBody);
    QWidget *widget3 = new QWidget();
    widget3->setLayout(rightBody);


    body->addWidget(widget1);
    body->addWidget(widget2);
    body->addWidget(widget3);


    layout->addLayout(header);
    layout->addLayout(body);

    this->setLayout(layout);
}

void MainDashboardTab::refreshUI()
{
    mainInfo->refreshMainInfoUI();
    incomesGraph->updateGraph();
    expensesGraph->updateGraph();
}

MainInfo::MainInfo(Tracker *tracker): tracker(tracker)
{
    QVBoxLayout *mainlayout = new QVBoxLayout();

    QHBoxLayout *left_body = new QHBoxLayout();

    QHBoxLayout *leftUpBody = new QHBoxLayout();


    std::string currentBalanceText = std::to_string(tracker->getUser()->getBalance());
    QLabel *currentBalanceIcon = new QLabel();
    QPixmap balanceIcon("../assets/balance.png");
    currentBalanceIcon->setPixmap(balanceIcon);
    currentBalanceValue = new QLabel(QString::fromStdString(currentBalanceText.substr(0,currentBalanceText.size()-4) + "$"));
    int percentages = tracker->getExpensesPercentage();
    currentBalanceValue->setToolTip(QString::fromStdString("Your current balance\nIncomes: " + std::to_string(tracker->getIncomesSum()) + "$\nExpenses: " + std::to_string(tracker->getExpensesSum()) + "$ (" + std::to_string(percentages) + "%)"));
    if (percentages > this->tracker->getUser()->getExpensePercentage())
    {
        currentBalanceValue->setStyleSheet("color: red");
    } else {
        currentBalanceValue->setStyleSheet("color: ");
    }



    QLabel *nextIncomesIcon = new QLabel();
    QPixmap incomesIcon("../assets/incomes.png");
    nextIncomesIcon->setPixmap(incomesIcon);
    std::string nextIncomesText = std::to_string(tracker->getUser()->getNextIncomes());
    nextIncomesValue = new QLabel(QString::fromStdString(nextIncomesText.substr(0,nextIncomesText.size()-4)));
    nextIncomesValue->setToolTip("Your future incomes");

    QLabel *nextExpansesIcon = new QLabel();
    QPixmap expansesIcon("../assets/expanses.png");
    nextExpansesIcon->setPixmap(expansesIcon);
    std::string nextExpansesText = std::to_string(tracker->getUser()->getNextExpanses());
    QLabel *nextExpansesValue = new QLabel(QString::fromStdString(nextExpansesText.substr(0,nextExpansesText.size()-4)));
    nextExpansesValue->setToolTip("Your future incomes");

    leftUpBody->addWidget(currentBalanceIcon);
    leftUpBody->addWidget(currentBalanceValue);
    leftUpBody->addSpacing(50);
    leftUpBody->addWidget(nextIncomesIcon);
    leftUpBody->addWidget(nextIncomesValue);
    leftUpBody->addSpacing(50);
    leftUpBody->addWidget(nextExpansesIcon);
    leftUpBody->addWidget(nextExpansesValue);

    this->setLayout(leftUpBody);
}

void MainInfo::refreshMainInfoUI()
{
    refreshCurrentBalance();
    refreshNextIncomeValue();
    refreshNextExpansesValue();
}

void MainInfo::refreshCurrentBalance()
{
    std::string balance = std::to_string(tracker->getUser()->getBalance());
    currentBalanceValue->setText(QString::fromStdString(balance.substr(0,balance.size()-4)));
    int percentages = tracker->getExpensesPercentage();
    currentBalanceValue->setToolTip(QString::fromStdString("Your current balance\nIncomes: " + std::to_string(tracker->getIncomesSum()) + "\nExpenses: " + std::to_string(tracker->getExpensesSum()) + "$ (" + std::to_string(percentages) + "%)"));
    if (percentages > this->tracker->getUser()->getExpensePercentage())
    {
        currentBalanceValue->setStyleSheet("color: red");
    } else {
        currentBalanceValue->setStyleSheet("color: ");
    }
    
}

void MainInfo::refreshNextIncomeValue()
{
    std::string nextIncomesText = std::to_string(tracker->getUser()->getNextIncomes());
    nextIncomesValue->setText(QString::fromStdString(nextIncomesText.substr(0,nextIncomesText.size()-4)));
}

void MainInfo::refreshNextExpansesValue()
{
    std::string nextExpansesText = std::to_string(tracker->getUser()->getNextExpanses());
    QLabel *nextExpansesValue = new QLabel(QString::fromStdString(nextExpansesText.substr(0,nextExpansesText.size()-4)));
}

MainGraph::MainGraph(Tracker* tracker)
{
    QPieSeries *series = new QPieSeries();
    series->append("Category 1", 30);
    series->append("Category 2", 50);
    series->append("Category 3", 20);
    series->append("Category 4", 20);
    series->append("Category 5", 20);
    series->append("Category 6", 20);
    series->append("Category 7", 20);

    qreal total = 0;
    for (QPieSlice *slice : series->slices())
        total += slice->value();

    for (QPieSlice *slice : series->slices()) {
        qreal percentage = 100*slice->value()/total;
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(percentage, 0, 'f', 1));
        slice->setLabelVisible(true);
        QFont font = slice->labelFont();
        font.setPointSize(10);
        slice->setLabelFont(font);
        slice->setLabelPosition(QPieSlice::LabelOutside);
    }


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pie Chart");
    chart->legend()->setVisible(false);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    series->setPieSize(0.4);
}

QChartView *MainGraph::getChart()
{
    return chartView;
}

IncomesGraph::IncomesGraph(Tracker *tracker): tracker(tracker)
{
    QPieSeries *series = new QPieSeries();

    qreal categories[incomeCategories.size()] = {};
    std::cout << "Tracker mode: " << tracker->getViewMode() << std::endl;
    for (auto& record: tracker->getIncomes()) {
        for (size_t i = 0; i < record.second.size(); i++)
        {
            QDate currentDate = QDate::currentDate();
            if (tracker->getViewMode() == 0)
            {
                if (record.first.month() != currentDate.month() || record.first.year() != currentDate.year())
                {
                    std::cout << "true" << std::endl;
                    continue;
                }    
            } else if (tracker->getViewMode() == 1){
                if (record.first < currentDate || record.first > currentDate.addDays(7))
                {
                    std::cout << "true" << std::endl;
                    continue;
                }
            } else if (tracker->getViewMode() == 2){
                if (record.first < currentDate || record.first > currentDate.addDays(14))
                {
                    std::cout << "true" << std::endl;
                    continue;
                }
            } else if (tracker->getViewMode() == 3){
                QDate nextMonth = currentDate.addMonths(1);
                if (record.first.month() != nextMonth.month() || record.first.year() != nextMonth.year())
                {
                    std::cout << "true" << std::endl;
                    continue;
                }
            }
            Income income = record.second[i];
            std::cout << income.getCategory() << " " << income.getValue() << std::endl;
        }
    }
    
    for (size_t i = 0; i < incomeCategories.size(); i++)
    {
        series->append(QString::fromStdString(incomeCategories[i].first),categories[i]);
    }
    
    qreal total = 0;
    for (QPieSlice *slice : series->slices())
        total += slice->value();

    for (QPieSlice *slice : series->slices()) {
        qreal percentage = 100*slice->value()/total;
        if (std::isnan(percentage)) {
            // Handle NaN value
            continue;
        }
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(percentage, 0, 'f', 1));
        
        if (percentage == 0.0)
        {
            slice->setLabelVisible(false);
        } else {
            slice->setLabelVisible(true);
        }
        QFont font = slice->labelFont();
        font.setPointSize(10);
        slice->setLabelFont(font);
    }


    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Incomes");

    chartView = new QChartView(chart);
    chartView->setBaseSize(0,0);
    chartView->setRenderHint(QPainter::Antialiasing);

    series->setPieSize(0.4);
}

void IncomesGraph::updateGraph() {
    std::cout << "Updating graph..." << std::endl;

    // Clear the existing series
    chart->removeAllSeries();

    // Create a new series
    chart->removeAllSeries();

    // Create a new series
    QPieSeries *newSeries = new QPieSeries();

    qreal categories[incomeCategories.size()] = {};

    for (auto& record: tracker->getIncomes()) {
        for (size_t i = 0; i < record.second.size(); i++)
        {
            QDate currentDate = QDate::currentDate();
            if (tracker->getViewMode() == 0)
            {
                if (record.first.month() != currentDate.month() || record.first.year() != currentDate.year())
                {
                    std::cout << "true" << std::endl;
                    continue;
                }    
            } else if (tracker->getViewMode() == 1){
                if (record.first < currentDate || record.first > currentDate.addDays(7))
                {
                    std::cout << "true" << std::endl;
                    continue;
                }
            } else if (tracker->getViewMode() == 2){
                if (record.first < currentDate || record.first > currentDate.addDays(14))
                {
                    std::cout << "true" << std::endl;
                    continue;
                }
            } else if (tracker->getViewMode() == 3){
                QDate nextMonth = currentDate.addMonths(1);
                if (record.first.month() != nextMonth.month() || record.first.year() != nextMonth.year())
                {
                    std::cout << "true" << std::endl;
                    continue;
                }
            }
            Income income = record.second[i];
            std::cout << income.getCategory() << " " << income.getValue() << std::endl;
        }
    }
    
    for (size_t i = 0; i < incomeCategories.size(); i++)
    {
        newSeries->append(QString::fromStdString(incomeCategories[i].first),categories[i]);
        std::cout << incomeCategories[i].first << " " << categories[i] << std::endl;

    }

    qreal total = 0;
    for (QPieSlice *slice : newSeries->slices())
        total += slice->value();

    for (QPieSlice *slice : newSeries->slices()) {
        qreal percentage = 100*slice->value()/total;
        if (std::isnan(percentage)) {
            // Handle NaN value
            continue;
        }
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(percentage, 0, 'f', 1));
        
        if (percentage == 0.0)
        {
            slice->setLabelVisible(false);
        } else {
            slice->setLabelVisible(true);
        }
        QFont font = slice->labelFont();
        font.setPointSize(10);
        slice->setLabelFont(font);
    }

    // Add the new series to the chart
    chart->addSeries(newSeries);
    newSeries->setPieSize(0.4);
    // Refresh the chart view
    chartView->repaint();
}
QChartView *IncomesGraph::getChart()
{
    return chartView;
}

ExpensesGraph::ExpensesGraph(Tracker *tracker): tracker(tracker)
{
    QPieSeries *series = new QPieSeries();

    qreal categories[expenseCategories.size()] = {};

    for (auto& record: tracker->getExpenses()) {
        for (size_t i = 0; i < record.second.size(); i++)
        {
            Expense expense = record.second[i];
            categories[expense.getCategory()] += expense.getValue();
        }
    }
    
    for (size_t i = 0; i < expenseCategories.size(); i++)
    {
        series->append(QString::fromStdString(expenseCategories[i].first),categories[i]);
    }
    
    qreal total = 0;
    for (QPieSlice *slice : series->slices())
        total += slice->value();

    for (QPieSlice *slice : series->slices()) {
        qreal percentage = 100*slice->value()/total;
        if (std::isnan(percentage)) {
            // Handle NaN value
            continue;
        }
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(percentage, 0, 'f', 1));
        
        if (percentage == 0.0)
        {
            slice->setLabelVisible(false);
        } else {
            slice->setLabelVisible(true);
        }
        QFont font = slice->labelFont();
        font.setPointSize(10);
        slice->setLabelFont(font);
    }


    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Expenses");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    series->setPieSize(0.4);
}

QChartView *ExpensesGraph::getChart()
{
    return chartView;
}

void ExpensesGraph::updateGraph()
{
    std::cout << "Updating graph..." << std::endl;

    // Clear the existing series
    chart->removeAllSeries();

    // Create a new series
    QPieSeries *newSeries = new QPieSeries();

    qreal categories[expenseCategories.size()] = {};

    for (auto& record: tracker->getExpenses()) {
        for (size_t i = 0; i < record.second.size(); i++)
        {
            Expense expense = record.second[i];
            categories[expense.getCategory()] += expense.getValue();
        }
    }
    
    for (size_t i = 0; i < expenseCategories.size(); i++)
    {
        newSeries->append(QString::fromStdString(expenseCategories[i].first),categories[i]);
    }
    
    qreal total = 0;
    for (QPieSlice *slice : newSeries->slices())
        total += slice->value();

    for (QPieSlice *slice : newSeries->slices()) {
        qreal percentage = 100*slice->value()/total;
        if (std::isnan(percentage)) {
            // Handle NaN value
            continue;
        }
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(percentage, 0, 'f', 1));
        
        if (percentage == 0.0)
        {
            slice->setLabelVisible(false);
        } else {
            slice->setLabelVisible(true);
        }
        QFont font = slice->labelFont();
        font.setPointSize(10);
        slice->setLabelFont(font);
    }

    // Add the new series to the chart
    chart->addSeries(newSeries);
    newSeries->setPieSize(0.4);
    // Refresh the chart view
    chartView->repaint();
}

ExpensesTab::ExpensesTab(Tracker *tracker, MainDashboardTab *mainDashboardTab)
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

    tableView = new ExpenseTableView(this,tracker);

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


    connect(addExpenses, &QPushButton::clicked, [this,tracker,addExpenses, mainDashboardTab]() {
        std::vector<ExpenseLine*> expenseToBeRemoved;
        int error_number = 0;
        int long_error = 0;
        for (auto& expenseLine : this->expenseLines) {
            QDate date = expenseLine->getDate();
            int category = expenseLine->getCategory()->currentIndex();
            int subcategory = expenseLine->getSubcategory()->currentIndex();
            QString text = expenseLine->getText();
            QString value = expenseLine->getValue();

            if (subcategory == -1) {
                error_number++;
                continue;
            }
            if (text == "")
            {
                text = QString::fromStdString(expenseCategories[category].first + " - " + expenseCategories[category].second[subcategory]);
            }
            if (value == "")
            {
                error_number++;
                continue;
            }
            if (text.length() > 30){
                long_error++;
                continue;
            }
            QList<QStandardItem*> items;
            items.append(new QStandardItem(date.toString("yyyy-MM-dd")));
            items.append(new QStandardItem(QString::fromStdString(expenseCategories[category].first)));
            items.append(new QStandardItem(QString::fromStdString(expenseCategories[category].second[subcategory])));
            items.append(new QStandardItem(text));
            items.append(new QStandardItem(value));

            std::pair<bool,std::string> added = tracker->addExpense(
                date,
                category,
                subcategory,
                text.toStdString(),
                value.toDouble()
            );
            if (added.first)
            {
                tableView->getModel()->appendRow(items);
                tableView->hideColumn(tableView->getModel()->columnCount());
                expenseToBeRemoved.push_back(expenseLine);
            }
        }
        for (size_t i = 0; i < expenseToBeRemoved.size(); i++)
        {
            this->removeExpensesFromAddList(expenseToBeRemoved[i]);
        }
        if(error_number > 0){
            QMessageBox::warning(this,"Warning", QString::fromStdString("Please fill in the fields in " + std::to_string(error_number) + " cases."));
        }
        if (long_error > 0)
        {
            QMessageBox::warning(this,"Warning", QString::fromStdString("Title can`t be long than 30 in " + std::to_string(long_error) + " cases."));
        }
        
        if (this->expenseLines.size() == 0)
        {
            addExpenses->hide();
            toggleButton->hide();
            scrollarea->hide(); 
            mainDashboardTab->refreshUI();
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

void ExpensesTab::removeExpensesFromAddList(ExpenseLine *expenseLine){
    auto it = std::find(expenseLines.begin(), expenseLines.end(), expenseLine);
    expenseLines.erase(it);
    expenseLine->deleteLater();
}

ExpenseLine::ExpenseLine(QVBoxLayout* layout, std::function<QWidget*()> createNewExpense, std::function<void(ExpenseLine*)> removeExpense) : layout(layout), position(position) {
    QGridLayout* lineLayout = new QGridLayout();

    dateEdit = new QDateEdit();
    dateEdit->setDisplayFormat("yyyy-MM-dd");
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

QComboBox* ExpenseLine::getCategory() {
    return comboBox1;
}

QComboBox* ExpenseLine::getSubcategory() {
    return comboBox2;
}

QString ExpenseLine::getText() {
    return textField->text();
}

QString ExpenseLine::getValue() {
    return valueField->text();
}

IncomesTab::IncomesTab(Tracker *tracker, MainDashboardTab* mainDashboardTab): tracker(tracker), mainDashboardTab(mainDashboardTab)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QHBoxLayout* header = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("New");
    QPushButton* addIncomes = new QPushButton("Add incomes");
    addIncomes->hide();


    header->addWidget(editButton,0);
    header->addWidget(addIncomes,0);
    header->addStretch(1);



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

    tableView = new IncomeTableView(this,tracker);

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

    connect(addIncomes, &QPushButton::clicked, [this,tracker, mainDashboardTab,addIncomes]() {
        std::vector<IncomeLine*> incomesToBeRemoved;
        int error_number = 0;
        int long_error = 0;
        for (auto& incomeLine : this->incomeLines) {
            QDate date = incomeLine->getDate();
            int category = incomeLine->getCategory()->currentIndex();
            int subcategory = incomeLine->getSubcategory()->currentIndex();
            QString text = incomeLine->getText();
            QString value = incomeLine->getValue();
            if (subcategory == -1) {
                error_number++;
                continue;
            }
            if (text == "")
            {
                text = QString::fromStdString(incomeCategories[category].first + " - " + incomeCategories[category].second[subcategory]);
            }
            if (value == "")
            {
                error_number++;
                continue;
            }
            if (text.length() > 30){
                long_error++;
                continue;
            }
            QList<QStandardItem*> items;
            items.append(new QStandardItem(date.toString("yyyy-MM-dd")));
            items.append(new QStandardItem(QString::fromStdString(incomeCategories[category].first)));
            items.append(new QStandardItem(QString::fromStdString(incomeCategories[category].second[subcategory])));
            items.append(new QStandardItem(text));
            items.append(new QStandardItem(value));

            std::pair<bool,std::string> added = tracker->addIncome(
                date,
                category,
                subcategory,
                text.toStdString(),
                value.toDouble()
            );
            if (added.first)
            {
                tableView->getModel()->appendRow(items);
                tableView->hideColumn(tableView->getModel()->columnCount());
                incomesToBeRemoved.push_back(incomeLine);
            }
        }
        for (auto& incomeLine : incomesToBeRemoved) {
            this->removeIncomeFromAddList(incomeLine);
        }
        if(error_number > 0){
            QMessageBox::warning(this,"Warning", QString::fromStdString("Please fill in the fields in " + std::to_string(error_number) + " cases."));
        }
        if (long_error > 0)
        {
            QMessageBox::warning(this,"Warning", QString::fromStdString("Title can`t be long than 30 in " + std::to_string(long_error) + " cases."));
        }
        
        if (this->incomeLines.size() == 0)
        {
            addIncomes->hide();
            toggleButton->hide();
            scrollarea->hide(); 
            mainDashboardTab->refreshUI();
        }
        std::cout << tracker->getIncomes().size() << std::endl;
    });   
}

void IncomesTab::removeIncomeFromAddList(IncomeLine *incomeLine){
    auto it = std::find(incomeLines.begin(), incomeLines.end(), incomeLine);
    incomeLines.erase(it);
    incomeLine->deleteLater();
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
    dateEdit->setDisplayFormat("yyyy-MM-dd");
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

QComboBox* IncomeLine::getCategory() {
    return comboBox1;
}

QComboBox* IncomeLine::getSubcategory() {
    return comboBox2;
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



    header->addWidget(editButton,0);
    header->addWidget(addGoalButton,0);
    header->addStretch(1);

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


