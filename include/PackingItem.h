#ifndef PACKING_ITEM_H
#define PACKING_ITEM_H

#include <string>
#include <nlohmann/json.hpp>

namespace travel_planner {

    struct PackingItem {
        std::string id;
        std::string itinerary_id;
        std::string name;
        int quantity;
        bool packed;

        // Default constructor
        PackingItem() : id(""), itinerary_id(""), name(""), quantity(1), packed(false) {}

        // Parameterized constructor
        PackingItem(const std::string& id, const std::string& itinerary_id,
            const std::string& name, int quantity = 1, bool packed = false)
            : id(id), itinerary_id(itinerary_id), name(name), quantity(quantity), packed(packed) {
        }
    };

    // JSON serialization
    inline void to_json(nlohmann::json& j, const PackingItem& item) {
        j = nlohmann::json{
            {"id", item.id},
            {"itinerary_id", item.itinerary_id},
            {"name", item.name},
            {"quantity", item.quantity},
            {"packed", item.packed}
        };
    }

    // JSON deserialization
    inline void from_json(const nlohmann::json& j, PackingItem& item) {
        j.at("id").get_to(item.id);
        j.at("itinerary_id").get_to(item.itinerary_id);
        j.at("name").get_to(item.name);
        j.at("quantity").get_to(item.quantity);
        j.at("packed").get_to(item.packed);
    }

} // namespace travel_planner

#endif // PACKING_ITEM_H