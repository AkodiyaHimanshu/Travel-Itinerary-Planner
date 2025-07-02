#include "StorageManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace travel_planner {

    // Constructor with default storage path
    StorageManager::StorageManager(const std::string& storage_path)
        : storage_path_(storage_path.empty() ? "data/itineraries.json" : storage_path) {
    }

    std::vector<Itinerary> StorageManager::loadAll() const {
        std::vector<Itinerary> itineraries;

        // Check if storage directory exists, create if needed
        std::filesystem::path dir_path = std::filesystem::path(storage_path_).parent_path();
        if (!std::filesystem::exists(dir_path)) {
            try {
                std::filesystem::create_directories(dir_path);
            }
            catch (const std::exception& e) {
                std::cerr << "Error creating directory " << dir_path.string() << ": " << e.what() << std::endl;
                return itineraries; // Return empty list on error
            }
        }

        // Check if file exists
        if (!std::filesystem::exists(storage_path_)) {
            // File doesn't exist, return empty list
            return itineraries;
        }

        // Open and read file
        try {
            std::ifstream file(storage_path_);
            if (!file.is_open()) {
                std::cerr << "Error opening file: " << storage_path_ << std::endl;
                return itineraries; // Return empty list on error
            }

            nlohmann::json j;
            file >> j;

            // Parse JSON into Itinerary objects
            if (j.is_array()) {
                for (const auto& item : j) {
                    try {
                        Itinerary itinerary = item.get<Itinerary>();
                        itineraries.push_back(itinerary);
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error parsing itinerary: " << e.what() << std::endl;
                        // Skip invalid entries but continue processing
                    }
                }
            }
            else {
                std::cerr << "Invalid JSON format: expected array" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error reading itineraries from " << storage_path_ << ": " << e.what() << std::endl;
            // Return empty list on error
        }

        return itineraries;
    }

    void StorageManager::saveAll(const std::vector<Itinerary>& itineraries) const {
        // Ensure directory exists
        std::filesystem::path dir_path = std::filesystem::path(storage_path_).parent_path();
        if (!std::filesystem::exists(dir_path)) {
            try {
                std::filesystem::create_directories(dir_path);
            }
            catch (const std::exception& e) {
                std::cerr << "Error creating directory " << dir_path.string() << ": " << e.what() << std::endl;
                return; // Exit on error
            }
        }

        try {
            // Convert itineraries to JSON
            nlohmann::json j = itineraries;

            // Write to file with pretty formatting
            std::ofstream file(storage_path_);
            if (!file.is_open()) {
                std::cerr << "Error opening file for writing: " << storage_path_ << std::endl;
                return;
            }

            file << j.dump(2); // Use indent of 2 spaces for pretty formatting

        }
        catch (const std::exception& e) {
            std::cerr << "Error saving itineraries to " << storage_path_ << ": " << e.what() << std::endl;
        }
    }

} // namespace travel_planner