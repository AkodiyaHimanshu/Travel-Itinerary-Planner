#ifndef TRAVEL_PLANNER_STORAGE_MANAGER_H
#define TRAVEL_PLANNER_STORAGE_MANAGER_H

#include "../include/Itinerary.h"
#include <vector>
#include <string>

namespace travel_planner {

    /**
     * Class for managing itinerary storage operations
     */
    class StorageManager {
    public:
        /**
         * Constructor
         * @param storage_path Path to the storage file
         */
        explicit StorageManager(const std::string& storage_path);

        /**
         * Loads all itineraries from storage
         * @return Vector containing all stored itineraries
         */
        std::vector<Itinerary> loadAll() const;

        /**
         * Saves all itineraries to storage
         * @param itineraries Vector of itineraries to save
         */
        void saveAll(const std::vector<Itinerary>& itineraries) const;

    private:
        std::string storage_path_; // Path to the storage file
    };

} // namespace travel_planner

#endif // TRAVEL_PLANNER_STORAGE_MANAGER_H