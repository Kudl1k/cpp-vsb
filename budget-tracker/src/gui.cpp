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
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QVBoxLayout* expanseLinesLayout = new QVBoxLayout(); // New layout for the ExpanseLine widgets

    QWidget* bottomWidget = new QWidget();

    QHBoxLayout* header = new QHBoxLayout();

    
    QPushButton* addExpanse = new QPushButton("Add Expanse");
    header->addWidget(addExpanse);
    toggleButton = new QCheckBox("Toggle adding new lines");
    connect(toggleButton, &QCheckBox::stateChanged, [this, expanseLinesLayout](int state) {
        if (state == Qt::Checked) {
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->show();
                expanseLine->getRemoveButton()->show();
                expanseLine->getAddButton()->show();
            }
        } else {
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->hide();
                expanseLine->getRemoveButton()->hide();
                expanseLine->getAddButton()->hide();
            }
            if (!this->expanseLines.empty()) {
                this->expanseLines.front()->show();
            }
        }
    });

    connect(addExpanse, &QPushButton::clicked, [this,expanseLinesLayout]() {
        if (this->expanseLines.size() == 0)
        {
            this->createNewExpanse(expanseLinesLayout);
        }
        
        if (toggleButton->isVisible())
        {
            toggleButton->hide();
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->hide();
            }
        } else {
            toggleButton->show();
            for (auto& expanseLine : this->expanseLines) {
                expanseLine->show();
            }
        }
    });

    mainLayout->addLayout(header);
    toggleButton->hide();
    mainLayout->addWidget(toggleButton);

    bottomWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding); // Make the bottom widget expandable

    mainLayout->addLayout(expanseLinesLayout); // Add the expanseLinesLayout to the mainLayout
    mainLayout->addWidget(bottomWidget);

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
    }
    if (expanseLines.size() == 1) {
        expanseLines.front()->getRemoveButton()->setDisabled(true);
    }
}

ExpanseLine::ExpanseLine(QVBoxLayout* layout, std::function<QWidget*()> createNewExpanse, std::function<void(ExpanseLine*)> removeExpanse) : layout(layout), position(position) {
    QGridLayout* lineLayout = new QGridLayout();

    QDateEdit* dateEdit = new QDateEdit();
    QComboBox* comboBox1 = new QComboBox();
    comboBox1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QComboBox* comboBox2 = new QComboBox();
    comboBox2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QLineEdit* textField = new QLineEdit();
    textField->setPlaceholderText("New thing");


    QLineEdit* valueField = new QLineEdit();
    valueField->setPlaceholderText("Enter value");

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
    connect(comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, comboBox2,textField,valueField](int index){
        comboBox2->clear();
        if(index >= 0 && index < categories.size()) {
            for(const auto& subcategory : categories[index].second) {
                comboBox2->addItem(QString::fromStdString(subcategory));
            }
        }
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