#include "gui.h"


GUI::GUI(QWidget* parent) : QMainWindow(parent)
{

    tracker = new Tracker();


    this->setWindowTitle("Budget Tracker - KUD0132");
    this->setMinimumSize(1280,720);
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

ExpansesTab::ExpansesTab(Tracker *tracker)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QVBoxLayout* expanseLinesLayout = new QVBoxLayout();
    scrollarea = new QScrollArea();
    scrollarea->hide();
    scrollarea->setFixedHeight(75);

    QVBoxLayout* bottomWidget = new QVBoxLayout();
    tableView = new ExpanseTableView(this);

    QHBoxLayout* header = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("New");
    header->addWidget(editButton,0);
    QPushButton* addExpanses = new QPushButton("Add expanses");
    addExpanses->hide();
    header->addWidget(addExpanses,0);
    header->addStretch(1);
    



    toggleButton = new QCheckBox("Toggle adding multiple expanses");
    connect(toggleButton, &QCheckBox::stateChanged, [this, expanseLinesLayout](int state) {
        if (state == Qt::Checked) {
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->show();
                expanseLine->getRemoveButton()->show();
                expanseLine->getAddButton()->show();
                scrollarea->setFixedHeight(200);

            }
        } else {
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->hide();
                expanseLine->getRemoveButton()->hide();
                expanseLine->getAddButton()->hide();
                scrollarea->setFixedHeight(75);
            }
            if (!this->expanseLines.empty()) {
                this->expanseLines.front()->show();
            }
        }
    });

    connect(editButton, &QPushButton::clicked, [this,expanseLinesLayout,addExpanses]() {
        if (this->expanseLines.size() == 0)
        {
            this->createNewExpanse(expanseLinesLayout);
        }
        
        if (toggleButton->isVisible())
        {
            addExpanses->hide();
            toggleButton->hide();
            scrollarea->hide(); 
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->hide();
            }
        } else {
            addExpanses->show();
            toggleButton->show();
            scrollarea->show();
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->show();
            }
        }
    });

    connect(addExpanses, &QPushButton::clicked, [this,tracker]() {
        for (auto& expanseLine : this->expanseLines) {
            QDate date = expanseLine->getDate();
            QString category = expanseLine->getCategory();
            QString subcategory = expanseLine->getSubcategory();
            QString text = expanseLine->getText();
            QString value = expanseLine->getValue();

            QList<QStandardItem*> items;
            items.append(new QStandardItem(date.toString()));
            items.append(new QStandardItem(category));
            items.append(new QStandardItem(subcategory));
            items.append(new QStandardItem(text));
            items.append(new QStandardItem(value));

            std::pair<bool,std::string> added = tracker->addExpanse(
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
                this->removeExpanse(expanseLine);
            }
        }
        std::cout << tracker->getExpanses().size() << std::endl;
    });



    mainLayout->addLayout(header);
    toggleButton->hide();
    mainLayout->addWidget(toggleButton);
    

    

    

    QWidget *scrollWidget = new QWidget();
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    expanseLinesLayout->setAlignment(Qt::AlignTop);
    scrollWidget->setLayout(expanseLinesLayout);


    scrollarea->setWidgetResizable(true);
    scrollarea->setWidget(scrollWidget);

    mainLayout->addWidget(scrollarea);
    mainLayout->addWidget(tableView);



    this->setLayout(mainLayout);
}

QWidget * ExpansesTab::createNewExpanse(QVBoxLayout* layout){
    ExpanseLine* newExpanseLine = new ExpanseLine(layout, [this, layout](){ return this->createNewExpanse(layout); }, [this](ExpanseLine* expanseLine){ this->removeExpanse(expanseLine); });
    this->expanseLines.push_back(newExpanseLine);
    layout->addWidget(newExpanseLine);
    std::cout << "Number of lines: " << this->expanseLines.size() << std::endl;
    
    if (this->expanseLines.size() == 1) {
        if (!this->toggleButton->isChecked())
        {
            this->expanseLines.front()->getRemoveButton()->hide();
            this->expanseLines.front()->getAddButton()->hide();
        } else {
            newExpanseLine->getRemoveButton()->setDisabled(true);
        }
    } else {
        if (!this->toggleButton->isChecked())
        {
            this->expanseLines.front()->getRemoveButton()->hide();
            this->expanseLines.front()->getAddButton()->hide();
        } else {
            this->expanseLines.front()->getRemoveButton()->setDisabled(false);
            this->expanseLines.front()->getAddButton()->show();
        }
    }
    return newExpanseLine;
}

void ExpansesTab::removeExpanse(ExpanseLine *expanseLine){
    auto it = std::find(expanseLines.begin(), expanseLines.end(), expanseLine);
    if (it != expanseLines.end()) {
        expanseLines.erase(it);
        expanseLine->deleteLater();
    }
    if (expanseLines.empty()) {
        toggleButton->hide();
        scrollarea->hide();
    } else if (expanseLines.size() == 1) {
        expanseLines.front()->getRemoveButton()->setDisabled(true);
    }
}

ExpanseLine::ExpanseLine(QVBoxLayout* layout, std::function<QWidget*()> createNewExpanse, std::function<void(ExpanseLine*)> removeExpanse) : layout(layout), position(position) {
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

    removeButton = new QPushButton("-");
    removeButton->setStyleSheet("color: red");
    removeButton->setToolTip("Remove");
    removeButton->setFixedWidth(30);
    addButton = new QPushButton("+");
    addButton->setStyleSheet("color: green");
    addButton->setToolTip("Add next expense");
    addButton->setFixedWidth(30);


    dateEdit->setDate(QDate::currentDate());

    for (size_t i = 0; i < categories.size(); i++)
    {
        comboBox1->addItem(QString::fromStdString(categories[i].first));
    }
    comboBox1->setCurrentIndex(0);
    connect(comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index){
        comboBox2->clear();
        if(index >= 0 && index < categories.size()) {
            for(const auto& subcategory : categories[index].second) {
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

    

    textField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(addButton, &QPushButton::clicked, [createNewExpanse](){
        createNewExpanse();
    });
    connect(removeButton, &QPushButton::clicked, [this,layout,removeExpanse]() {
        layout->removeWidget(this);
        removeExpanse(this);
        delete this;
    });

    lineLayout->addWidget(dateEdit, 0, 0);
    lineLayout->addWidget(comboBox1, 0, 1);
    lineLayout->addWidget(comboBox2, 0, 2);
    lineLayout->addWidget(textField, 0, 3);
    lineLayout->addWidget(valueField, 0, 4);
    lineLayout->addWidget(removeButton,0,5);
    lineLayout->addWidget(addButton, 0, 6);

    this->setLayout(lineLayout);
}

QPushButton* ExpanseLine::getRemoveButton() {
    return removeButton;
}

QPushButton * ExpanseLine::getAddButton()
{
    return addButton;
}


QDate ExpanseLine::getDate() {
    return dateEdit->date();
}

QString ExpanseLine::getCategory() {
    return comboBox1->currentText();
}

QString ExpanseLine::getSubcategory() {
    return comboBox2->currentText();
}

QString ExpanseLine::getText() {
    return textField->text();
}

QString ExpanseLine::getValue() {
    return valueField->text();
}

IncomesTab::IncomesTab(Tracker *tracker)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QVBoxLayout* incomeLinesLayout = new QVBoxLayout();
    scrollarea = new QScrollArea();
    scrollarea->hide();
    scrollarea->setFixedHeight(75);

    tableView = new IncomeTableView(this);

    QHBoxLayout* header = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("New");
    header->addWidget(editButton,0);
    QPushButton* addIncomes = new QPushButton("Add incomes");
    addIncomes->hide();
    header->addWidget(addIncomes,0);
    header->addStretch(1);

    toggleButton = new QCheckBox("Toggle adding multiple incomes");
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

    mainLayout->addLayout(header);
    toggleButton->hide();
    mainLayout->addWidget(toggleButton);

    QWidget *scrollWidget = new QWidget();
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    incomeLinesLayout->setAlignment(Qt::AlignTop);
    scrollWidget->setLayout(incomeLinesLayout);

    scrollarea->setWidgetResizable(true);
    scrollarea->setWidget(scrollWidget);

    mainLayout->addWidget(scrollarea);
    mainLayout->addWidget(tableView);

    this->setLayout(mainLayout);
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

    removeButton = new QPushButton("-");
    removeButton->setStyleSheet("color: red");
    removeButton->setToolTip("Remove");
    removeButton->setFixedWidth(30);
    addButton = new QPushButton("+");
    addButton->setStyleSheet("color: green");
    addButton->setToolTip("Add next income");
    addButton->setFixedWidth(30);


    dateEdit->setDate(QDate::currentDate());

    for (size_t i = 0; i < categories.size(); i++)
    {
        comboBox1->addItem(QString::fromStdString(categories[i].first));
    }
    comboBox1->setCurrentIndex(0);
    connect(comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index){
        comboBox2->clear();
        if(index >= 0 && index < categories.size()) {
            for(const auto& subcategory : categories[index].second) {
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

    

    textField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(addButton, &QPushButton::clicked, [createNewIncome](){
        createNewIncome();
    });
    connect(removeButton, &QPushButton::clicked, [this,layout,removeIncome]() {
        layout->removeWidget(this);
        removeIncome(this);
        delete this;
    });

    lineLayout->addWidget(dateEdit, 0, 0);
    lineLayout->addWidget(comboBox1, 0, 1);
    lineLayout->addWidget(comboBox2, 0, 2);
    lineLayout->addWidget(textField, 0, 3);
    lineLayout->addWidget(valueField, 0, 4);
    lineLayout->addWidget(removeButton,0,5);
    lineLayout->addWidget(addButton, 0, 6);

    this->setLayout(lineLayout);
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