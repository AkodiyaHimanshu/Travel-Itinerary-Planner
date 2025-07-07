#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "../include/Expense.h"

namespace travel_planner {

    class ExpenseManager {
    public:
        // Constructor with optional path parameter
        ExpenseManager(const std::string& storage_path = "data/expenses.json");

        // Load all expenses from storage
        std::vector<Expense> loadAll();

        // Save expenses to storage
        bool saveAll(const std::vector<Expense>& expenses);

        // Add a new expense
        bool addExpense(
            const std::string& itinerary_id,
            double amount,
            const std::string& category,
            const std::string& date,
            const std::string& description
        );

        // Get all expenses for a specific itinerary
        std::vector<Expense> listExpenses(const std::string& itinerary_id);

        // Generate category-wise expense summary for an itinerary
        std::map<std::string, double> summary(const std::string& itinerary_id);

        // Remove an expense by ID
        bool removeExpense(const std::string& expense_id);

    private:
        std::string data_file_path;
    };

} // namespace travel_planner

#endif // EXPENSE_MANAGER_H