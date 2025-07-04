#include "PackingManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <random>
#include <algorithm>

namespace travel_planner {

    PackingManager::PackingManager(const std::string& storage_path)
        : data_file_path(storage_path) {
    }

    // Generate a unique ID for a packing item
    static std::string generateUniqueId() {
        // Get current timestamp as part of the ID
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();

        // Generate some random characters
        std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> dist(0, chars.size() - 1);

        std::string random_str;
        for (int i = 0; i < 8; ++i) {
            random_str += chars[dist(generator)];
        }

        // Combine timestamp and random string
        return "pck_" + std::to_string(timestamp) + "_" + random_str;
    }

    std::vector<PackingItem> PackingManager::loadAll() const {
        std::vector<PackingItem> items;

        // Create parent directory if it doesn't exist
        std::filesystem::path path(data_file_path);
        std::filesystem::create_directories(path.parent_path());

        // Check if file exists
        if (!std::filesystem::exists(path)) {
            // Return empty vector if file doesn't exist yet
            return items;
        }

        try {
            std::ifstream file(data_file_path);
            if (!file.is_open()) {
                std::cerr << "Error: Unable to open packing items file for reading: "
                    << data_file_path << std::endl;
                return items;
            }

            nlohmann::json j;
            file >> j;

            if (j.is_array()) {
                for (const auto& item_json : j) {
                    PackingItem item;
                    from_json(item_json, item);
                    items.push_back(item);
                }
            }
        }
        catch (const nlohmann::json::exception& e) {
            std::cerr << "Error parsing packing items JSON: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading packing items: " << e.what() << std::endl;
        }

        return items;
    }

    void PackingManager::saveAll(const std::vector<PackingItem>& items) const {
        try {
            // Create parent directory if it doesn't exist
            std::filesystem::path path(data_file_path);
            std::filesystem::create_directories(path.parent_path());

            std::ofstream file(data_file_path);
            if (!file.is_open()) {
                std::cerr << "Error: Unable to open packing items file for writing: "
                    << data_file_path << std::endl;
                return;
            }

            nlohmann::json j = nlohmann::json::array();
            for (const auto& item : items) {
                nlohmann::json item_json;
                to_json(item_json, item);
                j.push_back(item_json);
            }

            file << j.dump(4); // Pretty print with 4-space indentation
        }
        catch (const std::exception& e) {
            std::cerr << "Error saving packing items: " << e.what() << std::endl;
        }
    }

    std::string PackingManager::addItem(const std::string& itinerary_id, const std::string& name, int quantity) {
        auto items = loadAll();

        // Generate a unique ID for the new item
        std::string id = generateUniqueId();

        // Create and add the new item
        PackingItem new_item(id, itinerary_id, name, quantity, false);
        items.push_back(new_item);

        // Save all items back to storage
        saveAll(items);

        return id;
    }

    bool PackingManager::markPacked(const std::string& item_id) {
        auto items = loadAll();

        // Find the item with the given ID
        auto it = std::find_if(items.begin(), items.end(),
            [&item_id](const PackingItem& item) { return item.id == item_id; });

        if (it == items.end()) {
            std::cerr << "Error: Packing item with ID " << item_id << " not found." << std::endl;
            return false;
        }

        // Toggle the packed status
        it->packed = !it->packed;

        // Save all items back to storage
        saveAll(items);

        return true;
    }

    bool PackingManager::removeItem(const std::string& item_id) {
        auto items = loadAll();

        // Find the item with the given ID
        auto it = std::find_if(items.begin(), items.end(),
            [&item_id](const PackingItem& item) { return item.id == item_id; });

        if (it == items.end()) {
            std::cerr << "Error: Packing item with ID " << item_id << " not found." << std::endl;
            return false;
        }

        // Remove the item
        items.erase(it);

        // Save all items back to storage
        saveAll(items);

        return true;
    }

} // namespace travel_planner