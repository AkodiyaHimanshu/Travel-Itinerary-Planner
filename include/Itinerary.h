#ifndef TRAVEL_PLANNER_ITINERARY_H
#define TRAVEL_PLANNER_ITINERARY_H

#include <string>
#include <nlohmann/json.hpp>

namespace travel_planner {

    /**
     * Structure representing a travel itinerary
     */
    struct Itinerary {
        std::string id;
        std::string name;
        std::string start_date;
        std::string end_date;
        std::string description;

        // Default constructor
        Itinerary() = default;

        // Parameterized constructor
        Itinerary(
            const std::string& id,
            const std::string& name,
            const std::string& start_date,
            const std::string& end_date,
            const std::string& description
        ) : id(id), name(name), start_date(start_date), end_date(end_date), description(description) {
        }
    };

    // JSON serialization function
    inline void to_json(nlohmann::json& j, const Itinerary& itinerary) {
        j = nlohmann::json{
            {"id", itinerary.id},
            {"name", itinerary.name},
            {"start_date", itinerary.start_date},
            {"end_date", itinerary.end_date},
            {"description", itinerary.description}
        };
    }

    // JSON deserialization function
    inline void from_json(const nlohmann::json& j, Itinerary& itinerary) {
        j.at("id").get_to(itinerary.id);
        j.at("name").get_to(itinerary.name);
        j.at("start_date").get_to(itinerary.start_date);
        j.at("end_date").get_to(itinerary.end_date);
        j.at("description").get_to(itinerary.description);
    }

} // namespace travel_planner

#endif // TRAVEL_PLANNER_ITINERARY_H