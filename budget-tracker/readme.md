# Budget Tracker

Budget tracker created in C++ using the Qt library. The tracker is able to track your expenses and incomes, set goals, and calculate investments. First, it will check if there is no data from the past session. If there is data, the user will be launched into the main window; if not, the user will go through the setup. The user will be asked several questions about their budgeting, such as the amount of income, distribution of expenses by healthy budget, etc. Then the user will be launched into the main window, which is divided into 5 sections. Project will be using serialization and deserialization.

## 1. Main Dashboard

The main dashboard will display main information such as current money for the month, a graph that will display expenses, expenses, and incomes in the current month, etc. There will also be quick actions for adding fast expenses or income.

## 2. Incomes

This tab will allow you to display your future incomes, but also your history. You can add incomes here one by one or in multiple records. You can also delete incomes in case you create one by mistake. There will also be filtering by the next week, two weeks, or month.

## 3. Expenses

Same as for the incomes.

## 4. Goals

A tab for making your dreams come true by achieving the goals you desire. You can set the priority of the goal, target date when you want to reach it, title, and also desired amount.

## 5. Investment Calculator

The source of your passive income. This tab will help you calculate how much money you will make in the future if you invest this amount for a certain time. It will also be connected with the incomes, so it will tell you if it is worth it for you or not.

### To-do

- [ ] Main dashboard. Create UI. (In Progrees)
- [ ] Expense and Income tabs, issue with deleting rows. (In Progrees)
- [ ] Goals tab. Implement functions in UI.
- [ ] Investment Calculator. Create its functionality and connect it with incomes. <br>

Link to [github repo](https://github.com/Kudl1k/cpp-vsb/tree/main/budget-tracker)