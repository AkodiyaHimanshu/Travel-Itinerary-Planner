#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include <nlohmann/json.hpp>

namespace travel_planner {

    struct Expense {
        std::string id;
        std::string itinerary_id;
        double amount;
        std::string category;
        std::string date;
        std::string description;

        Expense() = default;

        Expense(
            const std::string& id,
            const std::string& itinerary_id,
            double amount,
            const std::string& category,
            const std::string& date,
            const std::string& description
        ) : id(id), itinerary_id(itinerary_id), amount(amount),
            category(category), date(date), description(description) {
        }
    };

    // JSON serialization functions
    inline void to_json(nlohmann::json& j, const Expense& e) {
        j = nlohmann::json{
            {"id", e.id},
            {"itinerary_id", e.itinerary_id},
            {"amount", e.amount},
            {"category", e.category},
            {"date", e.date},
            {"description", e.description}
        };
    }

    // JSON deserialization function
    inline void from_json(const nlohmann::json& j, Expense& e) {
        j.at("id").get_to(e.id);
        j.at("itinerary_id").get_to(e.itinerary_id);
        j.at("amount").get_to(e.amount);
        j.at("category").get_to(e.category);
        j.at("date").get_to(e.date);
        j.at("description").get_to(e.description);
    }

} // namespace travel_planner

#endif // EXPENSE_H