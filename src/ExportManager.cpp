#include "ExportManager.h"
#include "PackingManager.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace fs = std::filesystem;
namespace travel_planner {

    ExportManager::ExportManager() {
        // Ensure the exports directory exists
        createDirectoryIfNeeded("exports");
    }

    ExportManager::~ExportManager() {
        // Cleanup if needed
    }

    bool ExportManager::exportItineraryMarkdown(const std::string& id, const std::string& path) {
        // Find the itinerary by ID
        StorageManager storageManager("data/itineraries.json");
        auto itineraries = storageManager.loadAll();

        auto it = std::find_if(itineraries.begin(), itineraries.end(),
            [&id](const Itinerary& itin) { return itin.id == id; });

        if (it == itineraries.end()) {
            std::cerr << "Itinerary with ID '" << id << "' not found." << std::endl;
            return false;
        }

        // Create export directory if needed
        std::string exportDir = path.empty() ? "exports" : path;
        if (!createDirectoryIfNeeded(exportDir)) {
            return false;
        }

        // Open output file
        std::string filename = getSafeFilename(it->name) + "_itinerary.md";
        std::string filepath = exportDir + "/" + filename;

        std::ofstream outFile(filepath);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << filepath << std::endl;
            return false;
        }

        // Write itinerary in Markdown format
        outFile << "# " << it->name << std::endl << std::endl;
        outFile << "**Itinerary ID:** " << it->id << std::endl;
        outFile << "**Start Date:** " << it->start_date << std::endl;
        outFile << "**End Date:** " << it->end_date << std::endl << std::endl;
        outFile << "## Description" << std::endl << std::endl;
        outFile << it->description << std::endl;

        outFile.close();
        std::cout << "Itinerary exported to " << filepath << std::endl;
        return true;
    }

    bool ExportManager::exportItineraryCSV(const std::string& id, const std::string& path) {
        // Find the itinerary by ID
        StorageManager storageManager("data/itineraries.json");
        auto itineraries = storageManager.loadAll();

        auto it = std::find_if(itineraries.begin(), itineraries.end(),
            [&id](const Itinerary& itin) { return itin.id == id; });

        if (it == itineraries.end()) {
            std::cerr << "Itinerary with ID '" << id << "' not found." << std::endl;
            return false;
        }

        // Create export directory if needed
        std::string exportDir = path.empty() ? "exports" : path;
        if (!createDirectoryIfNeeded(exportDir)) {
            return false;
        }

        // Open output file
        std::string filename = getSafeFilename(it->name) + "_itinerary.csv";
        std::string filepath = exportDir + "/" + filename;

        std::ofstream outFile(filepath);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << filepath << std::endl;
            return false;
        }

        // Write itinerary in CSV format
        outFile << "Item,Value" << std::endl;
        outFile << "ID," << quoteField(it->id) << std::endl;
        outFile << "Name," << quoteField(it->name) << std::endl;
        outFile << "Start Date," << quoteField(it->start_date) << std::endl;
        outFile << "End Date," << quoteField(it->end_date) << std::endl;
        outFile << "Description," << quoteField(it->description) << std::endl;

        outFile.close();
        std::cout << "Itinerary exported to " << filepath << std::endl;
        return true;
    }

    bool ExportManager::exportPackingMarkdown(const std::string& itin_id, const std::string& path) {
        // Find the itinerary by ID (for name)
        StorageManager storageManager("data/itineraries.json");
        auto itineraries = storageManager.loadAll();

        auto it = std::find_if(itineraries.begin(), itineraries.end(),
            [&itin_id](const Itinerary& itin) { return itin.id == itin_id; });

        if (it == itineraries.end()) {
            std::cerr << "Itinerary with ID '" << itin_id << "' not found." << std::endl;
            return false;
        }

        // Get packing items for the itinerary
        PackingManager packingManager;
        auto allItems = packingManager.loadAll();

        // Filter items for the specific itinerary
        std::vector<PackingItem> packingItems;
        for (const auto& item : allItems) {
            if (item.itinerary_id == itin_id) {
                packingItems.push_back(item);
            }
        }

        // Create export directory if needed
        std::string exportDir = path.empty() ? "exports" : path;
        if (!createDirectoryIfNeeded(exportDir)) {
            return false;
        }

        // Open output file
        std::string filename = getSafeFilename(it->name) + "_packing.md";
        std::string filepath = exportDir + "/" + filename;

        std::ofstream outFile(filepath);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << filepath << std::endl;
            return false;
        }

        // Write packing list in Markdown format
        outFile << "# Packing List for " << it->name << std::endl << std::endl;

        if (packingItems.empty()) {
            outFile << "No packing items found for this itinerary." << std::endl;
        }
        else {
            // Separate packed and unpacked items
            std::vector<PackingItem> packedItems;
            std::vector<PackingItem> unpackedItems;

            for (const auto& item : packingItems) {
                if (item.packed) {
                    packedItems.push_back(item);
                }
                else {
                    unpackedItems.push_back(item);
                }
            }

            // Display unpacked items first
            outFile << "## Unpacked Items" << std::endl << std::endl;
            if (unpackedItems.empty()) {
                outFile << "All items have been packed!" << std::endl << std::endl;
            }
            else {
                for (const auto& item : unpackedItems) {
                    outFile << "- [ ] " << item.quantity << "x " << item.name << " (ID: " << item.id << ")" << std::endl;
                }
                outFile << std::endl;
            }

            // Display packed items
            outFile << "## Packed Items" << std::endl << std::endl;
            if (packedItems.empty()) {
                outFile << "No items have been packed yet." << std::endl << std::endl;
            }
            else {
                for (const auto& item : packedItems) {
                    outFile << "- [x] " << item.quantity << "x " << item.name << " (ID: " << item.id << ")" << std::endl;
                }
                outFile << std::endl;
            }

            // Summary
            outFile << "## Summary" << std::endl << std::endl;
            outFile << "**Total Items:** " << packingItems.size() << std::endl;
            outFile << "**Packed:** " << packedItems.size() << std::endl;
            outFile << "**Unpacked:** " << unpackedItems.size() << std::endl;
            outFile << "**Progress:** " << (packingItems.empty() ? 0 : (packedItems.size() * 100 / packingItems.size())) << "%" << std::endl;
        }

        outFile.close();
        std::cout << "Packing list exported to " << filepath << std::endl;
        return true;
    }

    bool ExportManager::exportPackingCSV(const std::string& itin_id, const std::string& path) {
        // Find the itinerary by ID (for name)
        StorageManager storageManager("data/itineraries.json");
        auto itineraries = storageManager.loadAll();

        auto it = std::find_if(itineraries.begin(), itineraries.end(),
            [&itin_id](const Itinerary& itin) { return itin.id == itin_id; });

        if (it == itineraries.end()) {
            std::cerr << "Itinerary with ID '" << itin_id << "' not found." << std::endl;
            return false;
        }

        // Get packing items for the itinerary
        PackingManager packingManager;
        auto allItems = packingManager.loadAll();

        // Filter items for the specific itinerary
        std::vector<PackingItem> packingItems;
        for (const auto& item : allItems) {
            if (item.itinerary_id == itin_id) {
                packingItems.push_back(item);
            }
        }

        // Create export directory if needed
        std::string exportDir = path.empty() ? "exports" : path;
        if (!createDirectoryIfNeeded(exportDir)) {
            return false;
        }

        // Open output file
        std::string filename = getSafeFilename(it->name) + "_packing.csv";
        std::string filepath = exportDir + "/" + filename;

        std::ofstream outFile(filepath);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << filepath << std::endl;
            return false;
        }

        // Write packing list in CSV format
        outFile << "ID,Name,Quantity,Packed" << std::endl;

        for (const auto& item : packingItems) {
            outFile << quoteField(item.id) << ","
                << quoteField(item.name) << ","
                << item.quantity << ","
                << (item.packed ? "Yes" : "No") << std::endl;
        }

        outFile.close();
        std::cout << "Packing list exported to " << filepath << std::endl;
        return true;
    }

    bool ExportManager::exportExpenseMarkdown(const std::string& itin_id, const std::string& path) {
        // Find the itinerary by ID
        StorageManager storageManager("data/itineraries.json");
        auto itineraries = storageManager.loadAll();

        auto it = std::find_if(itineraries.begin(), itineraries.end(),
            [&itin_id](const Itinerary& itin) { return itin.id == itin_id; });

        if (it == itineraries.end()) {
            std::cerr << "Itinerary with ID '" << itin_id << "' not found." << std::endl;
            return false;
        }

        // Get expenses
        ExpenseManager expenseManager;
        auto expenses = expenseManager.listExpenses(itin_id);

        // Create export directory if needed
        std::string exportDir = path.empty() ? "exports" : path;
        if (!createDirectoryIfNeeded(exportDir)) {
            return false;
        }

        // Open output file
        std::string filename = getSafeFilename(it->name) + "_expenses.md";
        std::string filepath = exportDir + "/" + filename;

        std::ofstream outFile(filepath);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << filepath << std::endl;
            return false;
        }

        // Write expenses in Markdown format
        outFile << "# Expenses for " << it->name << std::endl << std::endl;

        if (expenses.empty()) {
            outFile << "No expenses recorded for this itinerary." << std::endl;
        }
        else {
            // Group expenses by category
            std::map<std::string, std::vector<Expense>> categorizedExpenses;
            double totalAmount = 0.0;

            for (const auto& expense : expenses) {
                categorizedExpenses[expense.category].push_back(expense);
                totalAmount += expense.amount;
            }

            for (const auto& [category, catExpenses] : categorizedExpenses) {
                double categoryTotal = 0.0;
                for (const auto& expense : catExpenses) {
                    categoryTotal += expense.amount;
                }

                outFile << "## " << category << " ($" << std::fixed << std::setprecision(2) << categoryTotal << ")" << std::endl << std::endl;

                outFile << "| Date | Description | Amount |" << std::endl;
                outFile << "|------|-------------|--------|" << std::endl;

                for (const auto& expense : catExpenses) {
                    outFile << "| " << expense.date << " | " << expense.description
                        << " | $" << std::fixed << std::setprecision(2) << expense.amount << " |" << std::endl;
                }

                outFile << std::endl;
            }

            outFile << "## Summary" << std::endl << std::endl;
            outFile << "**Total Expenses:** $" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
            outFile << "**Number of Expenses:** " << expenses.size() << std::endl;
        }

        outFile.close();
        std::cout << "Expenses exported to " << filepath << std::endl;
        return true;
    }

    bool ExportManager::exportExpenseCSV(const std::string& itin_id, const std::string& path) {
        // Find the itinerary by ID
        StorageManager storageManager("data/itineraries.json");
        auto itineraries = storageManager.loadAll();

        auto it = std::find_if(itineraries.begin(), itineraries.end(),
            [&itin_id](const Itinerary& itin) { return itin.id == itin_id; });

        if (it == itineraries.end()) {
            std::cerr << "Itinerary with ID '" << itin_id << "' not found." << std::endl;
            return false;
        }

        // Get expenses
        ExpenseManager expenseManager;
        auto expenses = expenseManager.listExpenses(itin_id);

        // Create export directory if needed
        std::string exportDir = path.empty() ? "exports" : path;
        if (!createDirectoryIfNeeded(exportDir)) {
            return false;
        }

        // Open output file
        std::string filename = getSafeFilename(it->name) + "_expenses.csv";
        std::string filepath = exportDir + "/" + filename;

        std::ofstream outFile(filepath);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << filepath << std::endl;
            return false;
        }

        // Write expenses in CSV format
        outFile << "ID,Date,Category,Amount,Description" << std::endl;

        for (const auto& expense : expenses) {
            outFile << quoteField(expense.id) << ","
                << quoteField(expense.date) << ","
                << quoteField(expense.category) << ","
                << std::fixed << std::setprecision(2) << expense.amount << ","
                << quoteField(expense.description) << std::endl;
        }

        outFile.close();
        std::cout << "Expenses exported to " << filepath << std::endl;
        return true;
    }

    bool ExportManager::createDirectoryIfNeeded(const std::string& path) {
        try {
            if (!fs::exists(path)) {
                if (!fs::create_directories(path)) {
                    std::cerr << "Failed to create directory: " << path << std::endl;
                    return false;
                }
            }
            return true;
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            return false;
        }
    }

    std::string ExportManager::getSafeFilename(const std::string& filename) {
        std::string result = filename;

        // Replace invalid filename characters with underscores
        const std::string invalidChars = "\\/:*?\"<>|";
        for (char c : invalidChars) {
            std::replace(result.begin(), result.end(), c, '_');
        }

        // Replace spaces with underscores for better file paths
        std::replace(result.begin(), result.end(), ' ', '_');

        return result;
    }

    std::string ExportManager::quoteField(const std::string& field) {
        // If field is empty, return empty quotes
        if (field.empty()) {
            return "\"\"";
        }

        // Check if field needs quoting (contains comma, quote, or newline)
        bool needsQuoting = false;
        bool hasQuotes = false;

        for (char c : field) {
            if (c == ',' || c == '\n' || c == '\r') {
                needsQuoting = true;
            }
            else if (c == '"') {
                needsQuoting = true;
                hasQuotes = true;
            }
        }

        // If no special handling needed, return as is
        if (!needsQuoting && !hasQuotes) {
            return field;
        }

        // If it has quotes, we need to double them
        std::string result = field;
        if (hasQuotes) {
            std::string::size_type pos = 0;
            while ((pos = result.find('"', pos)) != std::string::npos) {
                result.replace(pos, 1, "\"\"");
                pos += 2;  // Skip the two quotes we just inserted
            }
        }

        // Wrap in quotes
        return "\"" + result + "\"";
    }

} // namespace travel_planner