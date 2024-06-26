#include "expense.h"

std::vector<std::pair<std::string, std::vector<std::string>>> expenseCategories = {
        {"Housing", {"Mortgage or rent", "Property taxes", "Household repairs", "HOA fees"}},
        {"Transportation", {"Car payment", "Car warranty", "Gas", "Tires", "Maintenance and oil changes", "Parking fees", "Repairs", "Registration and DMV Fees"}},
        {"Food", {"Groceries", "Restaurants", "Pet food"}},
        {"Utilities", {"Electricity", "Water", "Garbage", "Phones", "Cable", "Internet"}},
        {"Clothing", {"Adult's clothing", "Adult's shoes", "Children's clothing", "Children's shoes"}},
        {"Medical/Healthcare", {"Primary care", "Dental care", "Specialty care (dermatologists, orthodontics, optometrists, etc.)", "Urgent care", "Medications", "Medical devices"}},
        {"Insurance", {"Health insurance", "Homeowner's or renter's insurance", "Home warranty or protection plan", "Auto insurance", "Life insurance", "Disability insurance"}},
        {"Household Items/Supplies", {"Toiletries", "Laundry detergent", "Dishwasher detergent", "Cleaning supplies", "Tools"}},
        {"Personal", {"Gym memberships", "Haircuts", "Salon services", "Cosmetics (like makeup or services like laser hair removal)", "Babysitter", "Subscriptions"}},
        {"Debt", {"Personal loans", "Student loans", "Credit cards"}},
        {"Retirement", {"Financial planning", "Investing"}},
        {"Education", {"Children’s college", "Your college", "School supplies", "Books"}},
        {"Savings", {"Emergency fund", "Big purchases like a new mattress or laptop", "Other savings"}},
        {"Gifts/Donations", {"Birthday", "Anniversary", "Wedding", "Christmas", "Special occasion", "Charities"}},
        {"Entertainment", {"Alcohol and/or bars", "Games", "Movies", "Concerts", "Vacations", "Subscriptions (Netflix, Amazon, Hulu, etc.)"}}
    };