#include "tracker.h"


Tracker::Tracker(User *user){
    this->user = user;
}

Tracker::~Tracker(){
    delete user;
}

User* Tracker::getUser(){
    return user;
}

float Tracker::getIncomesSum()
{
    float sum = 0;
    for (const auto& pair : incomes) {
        for (const auto& income : pair.second) {
            sum += income.getValue();
        }
    }
    return sum;
}

float Tracker::getExpensesSum()
{
    float sum = 0;
    for (const auto& pair : expenses) {
        for (const auto& expense : pair.second) {
            sum += expense.getValue();
        }
    }
    return sum;
}



int Tracker::getExpensesPercentage()
{
    int expensesSum = getExpensesSum();
    if (expensesSum > 0.0f)
    {
        return expensesSum / (this->getIncomesSum() / 100);
    }
    return 0.0f;
}

void Tracker::saveToFile()
{
    saveUserToFile();
    saveExpensesToFile();
    saveIncomesToFile();
}

void Tracker::saveUserToFile()
{
    std::ofstream outFile("../secret/user.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Could not open file for writing!" << std::endl;
        return;
    }

    // Write the name
    std::string name = user->getName();
    name.resize(20); // Ensure the name is exactly 20 characters
    outFile.write(name.c_str(), name.size());

    // Write the balance
    float balance = user->getBalance();
    outFile.write(reinterpret_cast<char*>(&balance), sizeof(balance));

    // Write the expensePercentage
    int expensePercentage = user->getExpensePercentage();
    outFile.write(reinterpret_cast<char*>(&expensePercentage), sizeof(expensePercentage));


    // Write the savingsPercentage
    int savingsPercentage = user->getSavingsPercentage();
    outFile.write(reinterpret_cast<char*>(&savingsPercentage), sizeof(savingsPercentage));

    outFile.close();
}

void Tracker::saveExpensesToFile()
{
    std::ofstream outFile("../secret/expenses.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Could not open file for writing!" << std::endl;
        return;
    }

    for (const auto& pair : allExpenses) {
        for (const auto& expense : pair.second) {
            // Write the date
            std::string date = pair.first.toString("yyyy-MM-dd").toStdString();
            if (date.size() != 10) {
                std::cerr << "Invalid date for expense: " << expense.getTitle() << std::endl;
                continue;
            }
            date.resize(10); // Ensure the date is exactly 10 characters
            outFile.write(date.c_str(), date.size());

            // Write the category
            int category = expense.getCategory();
            outFile.write(reinterpret_cast<char*>(&category), sizeof(category));

            // Write the subcategory
            int subcategory = expense.getSubcategory();
            outFile.write(reinterpret_cast<char*>(&subcategory), sizeof(subcategory));

            // Write the title
            std::string title = expense.getTitle();
            title.resize(20); // Ensure the title is exactly 20 characters
            outFile.write(title.c_str(), title.size());

            // Write the value
            double value = expense.getValue();
            outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
    }

    outFile.close();
}

void Tracker::saveIncomesToFile()
{
    std::ofstream outFile("../secret/incomes.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Could not open file for writing!" << std::endl;
        return;
    }

    for (const auto& pair : allIncomes) {
        for (const auto& income : pair.second) {
            std::string date = pair.first.toString("yyyy-MM-dd").toStdString();
            if (date.size() != 10) {
                std::cerr << "Invalid date for income: " << income.getTitle() << std::endl;
                continue;
            }
            date.resize(10);
            outFile.write(date.c_str(), date.size());

            int category = income.getCategory();
            outFile.write(reinterpret_cast<char*>(&category), sizeof(category));

            int subcategory = income.getSubcategory();
            outFile.write(reinterpret_cast<char*>(&subcategory), sizeof(subcategory));

            std::string title = income.getTitle();
            title.resize(20);
            outFile.write(title.c_str(), title.size());

            double value = income.getValue();
            outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
    }

    outFile.close();
}

void Tracker::loadFromFile()
{
    loadUserFromFile();
    loadExpensesFromFile();
    loadIncomesFromFile();
}

void Tracker::setViewMode(int mode)
{
    this->viewmode = mode;
}

int Tracker::getViewMode()
{
    return this->viewmode;
}

void Tracker::loadUserFromFile()
{
    std::ifstream inFile("../secret/user.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Could not open file for reading!" << std::endl;
        return;
    }

    std::string name(20, '\0');
    inFile.read(&name[0], name.size());

    name.erase(std::find(name.begin(), name.end(), '\0'), name.end());

    float balance;
    inFile.read(reinterpret_cast<char*>(&balance), sizeof(balance));

    int expensePercentage;
    inFile.read(reinterpret_cast<char*>(&expensePercentage), sizeof(expensePercentage));

    int savingsPercentage;
    inFile.read(reinterpret_cast<char*>(&savingsPercentage), sizeof(savingsPercentage));

    inFile.close();

    std::cout << "Name: " << name << std::endl;
    std::cout << "Balance: " << balance << std::endl;
    std::cout << "Expense Percentage: " << expensePercentage << std::endl;
    std::cout << "Savings Percentage: " << savingsPercentage << std::endl;
    this->user = new User(name, expensePercentage, savingsPercentage);
}

void Tracker::loadExpensesFromFile()
{
    std::ifstream inFile("../secret/expenses.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Could not open file for reading!" << std::endl;
        return;
    }

    while (inFile.peek() != EOF) {
        // Read the date
        std::string dateStr(10, '\0');
        inFile.read(&dateStr[0], dateStr.size());
        QDate date = QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd");

        // Read the category
        int category;
        inFile.read(reinterpret_cast<char*>(&category), sizeof(category));

        // Read the subcategory
        int subcategory;
        inFile.read(reinterpret_cast<char*>(&subcategory), sizeof(subcategory));

        // Read the title
        std::string title(20, '\0');
        inFile.read(&title[0], title.size());
        title.erase(std::find(title.begin(), title.end(), '\0'), title.end());

        // Read the value
        double value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));

        std::cout << "Date: " << date.toString("yyyy-MM-dd").toStdString() << std::endl;
        std::cout << "Category: " << category << std::endl;
        std::cout << "Subcategory: " << subcategory << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Value: " << value << std::endl;

        addExpense(date, category, subcategory, title, value);
    }

    inFile.close();
}

void Tracker::loadIncomesFromFile()
{
    std::ifstream inFile("../secret/incomes.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Could not open file for reading!" << std::endl;
        return;
    }

    while (inFile.peek() != EOF) {
        std::string dateStr(10, '\0');
        inFile.read(&dateStr[0], dateStr.size());
        QDate date = QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd");

        int category;
        inFile.read(reinterpret_cast<char*>(&category), sizeof(category));

        int subcategory;
        inFile.read(reinterpret_cast<char*>(&subcategory), sizeof(subcategory));

        std::string title(20, '\0');
        inFile.read(&title[0], title.size());
        title.erase(std::find(title.begin(), title.end(), '\0'), title.end());

        double value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));

        addIncome(date, category, subcategory, title, value);
    }

    inFile.close();
}

std::pair<bool, std::string> Tracker::addExpense(QDate date, int category_name, int subcategory_name, std::string title, double value){

    if (title == "")
    {
        title = category_name + " - " + subcategory_name;
    }
    Expense *e = new Expense(date,category_name,subcategory_name,title,value);
    
    if (e->getDate() <= QDate::currentDate())
    {
        expenses[date].push_back(*e);
        user->removeBalance(e->getValue());
    } else {
        futureExpanses[date].push_back(*e);
        user->addNextExpanses(e->getValue());
    }
    allExpenses[date].push_back(*e);
    return std::make_pair(true,"Expense was successfully added");
}

std::pair<bool, std::string> Tracker::addIncome(QDate date, int category_name,int subcategory_name, std::string title, double value){

    if (title.empty())
    {
        title = category_name + " - " + subcategory_name;
    }
    Income *i = new Income(date,category_name,subcategory_name,title,value);
    
    if (i->getDate() <= QDate::currentDate())
    {
        incomes[date].push_back(*i);
        user->addBalance(i->getValue());
    } else {
        futureIncomes[date].push_back(*i);
        user->addNextIncome(i->getValue());
    }
    allIncomes[date].push_back(*i);
    return std::make_pair(true,"Income was successfully added");
}

std::map<QDate,std::vector<Expense>> Tracker::getExpenses()
{
    return expenses;
}

std::map<QDate, std::vector<Expense>> Tracker::getFutureExpenses()
{
    return futureExpanses;
}

std::map<QDate, std::vector<Expense>> Tracker::getAllExpenses()
{
    return allExpenses;
}

std::map<QDate,std::vector<Income>> Tracker::getIncomes()
{
    return incomes;
}

std::map<QDate, std::vector<Income>> Tracker::getFutureIncomes()
{
    return futureIncomes;
}

std::map<QDate, std::vector<Income>> Tracker::getAllIncomes()
{
    return allIncomes;
}

int Tracker::generateNewExpenseId()
{
    return expenses.size();
}

int Tracker::generateNewIncomeId()
{
    return incomes.size();
}