#include "ExpenseManager.h"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

namespace travel_planner {

    ExpenseManager::ExpenseManager(const std::string& storage_path)
        : data_file_path(storage_path) {
    }

    std::vector<Expense> ExpenseManager::loadAll() {
        std::vector<Expense> expenses;

        if (!std::filesystem::exists(data_file_path)) {
            return expenses;  // Return empty list if file doesn't exist
        }

        try {
            std::ifstream file(data_file_path);
            if (file.is_open()) {
                nlohmann::json data = nlohmann::json::parse(file);
                expenses = data.get<std::vector<Expense>>();
                file.close();
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading expenses: " << e.what() << std::endl;
            // Return empty list on error
        }

        return expenses;
    }

    bool ExpenseManager::saveAll(const std::vector<Expense>& expenses) {
        try {
            // Create directories if they don't exist
            std::filesystem::path path(data_file_path);
            if (path.has_parent_path() && !std::filesystem::exists(path.parent_path())) {
                std::filesystem::create_directories(path.parent_path());
            }

            std::ofstream file(data_file_path);
            if (file.is_open()) {
                nlohmann::json data = expenses;
                file << data.dump(4);  // Pretty print with 4-space indent
                file.close();
                return true;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error saving expenses: " << e.what() << std::endl;
        }

        return false;
    }

    bool ExpenseManager::addExpense(
        const std::string& itinerary_id,
        double amount,
        const std::string& category,
        const std::string& date,
        const std::string& description
    ) {
        // Generate a unique ID for the new expense
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 35);

        const std::string chars = "0123456789abcdefghijklmnopqrstuvwxyz";
        std::string random_str;
        for (int i = 0; i < 6; i++) {
            random_str += chars[dist(gen)];
        }

        // Format: timestamp-randomchars
        std::string id = std::to_string(timestamp) + "-" + random_str;

        // Create the expense object
        Expense expense(id, itinerary_id, amount, category, date, description);

        // Load existing expenses
        std::vector<Expense> expenses = loadAll();

        // Add the new expense
        expenses.push_back(expense);

        // Save updated expenses list
        return saveAll(expenses);
    }

    std::vector<Expense> ExpenseManager::listExpenses(const std::string& itinerary_id) {
        std::vector<Expense> all_expenses = loadAll();
        std::vector<Expense> filtered_expenses;

        // Filter expenses by itinerary_id
        std::copy_if(all_expenses.begin(), all_expenses.end(),
            std::back_inserter(filtered_expenses),
            [&itinerary_id](const Expense& e) {
                return e.itinerary_id == itinerary_id;
            });

        return filtered_expenses;
    }

    std::map<std::string, double> ExpenseManager::summary(const std::string& itinerary_id) {
        std::vector<Expense> expenses = listExpenses(itinerary_id);
        std::map<std::string, double> category_totals;

        // Sum expenses by category
        for (const auto& expense : expenses) {
            category_totals[expense.category] += expense.amount;
        }

        return category_totals;
    }

    bool ExpenseManager::removeExpense(const std::string& expense_id) {
        // Load all expenses
        std::vector<Expense> expenses = loadAll();

        // Check if the list is empty
        if (expenses.empty()) {
            return false;  // Nothing to remove
        }

        // Find the expense to remove
        auto it = std::find_if(expenses.begin(), expenses.end(),
            [&expense_id](const Expense& e) {
                return e.id == expense_id;
            });

        // Check if the expense was found
        if (it == expenses.end()) {
            return false;  // Expense not found
        }

        // Remove the expense
        expenses.erase(it);

        // Save the updated list
        return saveAll(expenses);
    }

} // namespace travel_planner