#ifndef EXPORT_MANAGER_H
#define EXPORT_MANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include "../include/Itinerary.h"
#include "StorageManager.h"
#include "ExpenseManager.h"

namespace travel_planner {

    class ExportManager {
    public:
        ExportManager();
        ~ExportManager();

        // Itinerary export methods
        bool exportItineraryMarkdown(const std::string& id, const std::string& path);
        bool exportItineraryCSV(const std::string& id, const std::string& path);

        // Packing list export methods
        bool exportPackingMarkdown(const std::string& itin_id, const std::string& path);
        bool exportPackingCSV(const std::string& itin_id, const std::string& path);

        // Expense export methods
        bool exportExpenseMarkdown(const std::string& itin_id, const std::string& path);
        bool exportExpenseCSV(const std::string& itin_id, const std::string& path);

    private:
        // Helper methods
        bool createDirectoryIfNeeded(const std::string& path);
        std::string getSafeFilename(const std::string& filename);
        std::string quoteField(const std::string& field);
    };

} // namespace travel_planner

#endif // EXPORT_MANAGER_H