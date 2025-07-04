#ifndef PACKING_MANAGER_H
#define PACKING_MANAGER_H

#include "../include/PackingItem.h"
#include <vector>
#include <string>

namespace travel_planner {

    class PackingManager {
    public:
        // Constructor with default storage path
        explicit PackingManager(const std::string& storage_path = "data/packing_items.json");

        // Load all packing items from storage
        std::vector<PackingItem> loadAll() const;

        // Save all packing items to storage
        void saveAll(const std::vector<PackingItem>& items) const;

        // Add a new packing item
        std::string addItem(const std::string& itinerary_id, const std::string& name, int quantity = 1);

        // Mark an item as packed/unpacked (toggles state)
        bool markPacked(const std::string& item_id);

        // Remove a packing item
        bool removeItem(const std::string& item_id);

    private:
        std::string data_file_path;
    };

} // namespace travel_planner

#endif // PACKING_MANAGER_H