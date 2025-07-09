#ifndef TRAVEL_PLANNER_ITINERARY_H
#define TRAVEL_PLANNER_ITINERARY_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace travel_planner {
    struct Itinerary {
        std::string id;
        std::string name;
        std::string start_date;
        std::string end_date;
        std::string description;
        std::vector<std::string> tags;  // New field for tags
        bool is_favorite = false;

        Itinerary() = default;

        Itinerary(const std::string& id, const std::string& name,
            const std::string& start_date, const std::string& end_date,
            const std::string& description,
            const std::vector<std::string>& tags = {})  // Updated constructor
            : id(id), name(name), start_date(start_date),
            end_date(end_date), description(description), tags(tags), is_favorite(is_favorite) {
        }
    };

    // Updated JSON serialization
    inline void to_json(nlohmann::json& j, const Itinerary& itinerary) {
        j = nlohmann::json{
            {"id", itinerary.id},
            {"name", itinerary.name},
            {"start_date", itinerary.start_date},
            {"end_date", itinerary.end_date},
            {"description", itinerary.description},
            {"tags", itinerary.tags},  // Added tags serialization
            { "is_favorite", itinerary.is_favorite }
        };
    }

    // Updated JSON deserialization
    inline void from_json(const nlohmann::json& j, Itinerary& itinerary) {
        j.at("id").get_to(itinerary.id);
        j.at("name").get_to(itinerary.name);
        j.at("start_date").get_to(itinerary.start_date);
        j.at("end_date").get_to(itinerary.end_date);
        j.at("description").get_to(itinerary.description);
        j.at("tags").get_to(itinerary.tags);  // Added tags deserialization

        // Handle backward compatibility
        if (j.contains("is_favorite")) {
            j.at("is_favorite").get_to(itinerary.is_favorite);
        }
        else {
            itinerary.is_favorite = false; // Default value if not present
        }
    }
}

#endif // TRAVEL_PLANNER_ITINERARY_H