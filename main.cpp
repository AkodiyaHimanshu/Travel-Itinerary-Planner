// Travel Itinerary Planner.cpp : Defines the entry point for the application.
//

#include "main.h"

using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include "include/version.h"
#include "include/Itinerary.h"
#include "src/StorageManager.h"

// Function declarations
void displayBanner();
void displayHelp();
void displayVersion();
bool hasOption(int argc, char* argv[], const std::string& option);
bool isKnownOption(const std::string& option);
std::string findUnknownOption(int argc, char* argv[]);
std::string generateUUID();
void addItinerary();
void listItineraries();
std::string promptInput(const std::string& prompt, bool allowEmpty = false);

int main(int argc, char* argv[]) {
    displayBanner();

    // Check for unknown options
    std::string unknownOption = findUnknownOption(argc, argv);
    if (!unknownOption.empty()) {
        std::cerr << "Error: Unknown option '" << unknownOption << "'" << std::endl;
        displayHelp();
        return 1;
    }

    // Help option
    if (argc == 1 || hasOption(argc, argv, "--help") || hasOption(argc, argv, "-h")) {
        displayHelp();
        return 0;
    }

    // Version option
    if (hasOption(argc, argv, "--version")) {
        displayVersion();
        return 0;
    }

    // Add itinerary option
    if (hasOption(argc, argv, "add")) {
        addItinerary();
        return 0;
    }

    if (hasOption(argc, argv, "list")) {
        listItineraries();
        return 0;
    }

    // If no valid command is provided
    std::cerr << "Error: Invalid command" << std::endl;
    displayHelp();
    return 1;
}

// Display the application banner
void displayBanner() {
    std::string title = "Travel Itinerary Planner CLI";
    std::string separator(title.length(), '=');
    std::cout << separator << std::endl;
    std::cout << title << std::endl;
    std::cout << separator << std::endl << std::endl;
}

// Display help information
void displayHelp() {
    std::cout << "Usage: travel_planner [OPTION]..." << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help, -h    Display this help message" << std::endl;
    std::cout << "  --version     Display version information" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  add           Add a new itinerary" << std::endl;
    std::cout << "  list          List all itineraries" << std::endl;
    std::cout << "  view ID       View details of a specific itinerary" << std::endl;
    std::cout << "  edit ID       Edit an existing itinerary" << std::endl;
    std::cout << "  delete ID     Delete an itinerary" << std::endl;
}

// Display version information
void displayVersion() {
    std::cout << "Travel Itinerary Planner v" << TRAVEL_PLANNER_VERSION << std::endl;
    std::cout << "Build date: " << __DATE__ << std::endl;
}

// Check if a specific option is present in the arguments
bool hasOption(int argc, char* argv[], const std::string& option) {
    return std::find_if(argv + 1, argv + argc, [&option](const char* arg) {
        return option == arg;
        }) != (argv + argc);
}

// Check if an option is recognized by the program
bool isKnownOption(const std::string& option) {
    static const std::vector<std::string> knownOptions = {
        "--help", "-h", "--version", "add", "list", "view", "edit", "delete"
    };

    return std::find(knownOptions.begin(), knownOptions.end(), option) != knownOptions.end();
}

// Find any unknown option in the arguments
std::string findUnknownOption(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--", 0) == 0 && !isKnownOption(arg)) {
            return arg;
        }
    }
    return "";
}

// Generate a simple UUID for itinerary IDs
std::string generateUUID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static const char* hex_chars = "0123456789abcdef";

    std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";

    for (char& c : uuid) {
        if (c == 'x') {
            c = hex_chars[dis(gen)];
        }
        else if (c == 'y') {
            c = hex_chars[(dis(gen) & 0x3) | 0x8];
        }
    }

    return uuid;
}

// Prompt user for input with validation
std::string promptInput(const std::string& prompt, bool allowEmpty) {
    std::string input;
    bool valid = false;

    while (!valid) {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (input.empty() && !allowEmpty) {
            std::cout << "Input cannot be empty. Please try again." << std::endl;
        }
        else {
            valid = true;
        }
    }

    return input;
}

// Add a new itinerary with user prompts
void addItinerary() {
    std::cout << "=== Add New Itinerary ===" << std::endl;

    // Generate a unique ID
    std::string id = generateUUID();

    // Collect itinerary details from user
    std::string name = promptInput("Enter itinerary name: ");
    std::string start_date = promptInput("Enter start date (YYYY-MM-DD): ");
    std::string end_date = promptInput("Enter end date (YYYY-MM-DD): ");
    std::string description = promptInput("Enter description (press Enter when done): ", true);

    // Create the itinerary object
    travel_planner::Itinerary newItinerary(id, name, start_date, end_date, description);

    // Load existing itineraries
    travel_planner::StorageManager storageManager("");  // Using default path
    std::vector<travel_planner::Itinerary> itineraries = storageManager.loadAll();

    // Add the new itinerary
    itineraries.push_back(newItinerary);

    // Save all itineraries
    storageManager.saveAll(itineraries);

    std::cout << "Itinerary added successfully with ID: " << id << std::endl;
}

void listItineraries() {
    std::cout << "Loading itineraries...\n";

    // Create storage manager with default path
	travel_planner::StorageManager storageManager("data/itineraries.json");

    // Load all itineraries
    std::vector<travel_planner::Itinerary> itineraries = storageManager.loadAll();

    if (itineraries.empty()) {
        std::cout << "No itineraries found.\n";
        return;
    }

    // Calculate column widths (minimum width plus some padding)
    size_t idWidth = 10;  // Minimum width for ID column
    size_t nameWidth = 20; // Minimum width for Name column

    // Adjust column widths based on actual data
    for (const auto& itinerary : itineraries) {
        idWidth = std::max(idWidth, itinerary.id.length() + 2);
        nameWidth = std::max(nameWidth, itinerary.name.length() + 2);
    }

    // Print table header
    std::cout << std::string(idWidth + nameWidth + 3, '-') << '\n';
    std::cout << "| " << std::left << std::setw(idWidth) << "ID"
        << "| " << std::setw(nameWidth) << "Name" << "|\n";
    std::cout << std::string(idWidth + nameWidth + 3, '-') << '\n';

    // Print each itinerary
    for (const auto& itinerary : itineraries) {
        std::cout << "| " << std::left << std::setw(idWidth) << itinerary.id
            << "| " << std::setw(nameWidth) << itinerary.name << "|\n";
    }

    // Print table footer
    std::cout << std::string(idWidth + nameWidth + 3, '-') << '\n';
    std::cout << itineraries.size() << " itinerary/ies found.\n";
}