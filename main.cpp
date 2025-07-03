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
void viewItinerary(const std::string& id);
bool deleteItinerary(const std::string& id);
bool addTagToItinerary(const std::string& id, const std::string& tag, bool& alreadyExists);
bool removeTagFromItinerary(const std::string& id, const std::string& tag, bool& tagExists);
void listTagsForItinerary(const std::string& id);
void searchItineraries(const std::string& namePattern);
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
    else if (argc == 1 || hasOption(argc, argv, "--help") || hasOption(argc, argv, "-h")) {
        displayHelp();
        return 0;
    }

    // Version option
    else if (hasOption(argc, argv, "--version")) {
        displayVersion();
        return 0;
    }

    // Add itinerary option
    else if (hasOption(argc, argv, "add") && argc < 3) {
        addItinerary();
        return 0;
    }

    else if (hasOption(argc, argv, "list") && argc < 3) {
        listItineraries();
        return 0;
    }

    else if (hasOption(argc, argv, "view") && argc < 3) {
        if (argc < 3) {
            std::cerr << "Error: 'view' command requires an itinerary ID\n";
            std::cerr << "Usage: " << argv[0] << " view <itinerary_id>\n";
            return 1;
        }

        std::string id = argv[2];
        viewItinerary(id);
        return 0;
    }

    else if (hasOption(argc, argv, "delete") && argc < 3) {
        if (argc < 3) {
            std::cerr << "Error: 'delete' command requires an itinerary ID\n";
            std::cerr << "Usage: " << argv[0] << " delete <itinerary_id>\n";
            return 1;
        }

        std::string id = argv[2];

        // Confirm deletion with the user
        char confirm;
        std::cout << "Are you sure you want to delete the itinerary with ID '" << id << "'? (y/n): ";
        std::cin >> confirm;

        if (tolower(confirm) != 'y') {
            std::cout << "Deletion cancelled.\n";
            return 0;
        }

        if (!deleteItinerary(id)) {
            return 1; // Return error status if deletion failed
        }
        return 0;
    }

    else if (argc >= 5 && std::string(argv[1]) == "tag" && std::string(argv[2]) == "add") {
        std::string id = argv[3];
        std::string tag = argv[4];

        bool alreadyExists = false;
        if (addTagToItinerary(id, tag, alreadyExists)) {
            if (alreadyExists) {
                std::cout << "Tag \"" << tag << "\" already exists for itinerary." << std::endl;
            }
            else {
                std::cout << "Tag \"" << tag << "\" added successfully to itinerary." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Itinerary with ID \"" << id << "\" not found." << std::endl;
            return 1;
        }
        return 0;
    }

    else if (argc >= 5 && std::string(argv[1]) == "tag" && std::string(argv[2]) == "remove") {
        std::string id = argv[3];
        std::string tag = argv[4];

        bool tagExists = false;
        if (removeTagFromItinerary(id, tag, tagExists)) {
            if (tagExists) {
                std::cout << "Tag \"" << tag << "\" removed successfully from itinerary." << std::endl;
            }
            else {
                std::cout << "Tag \"" << tag << "\" does not exist for this itinerary." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Itinerary with ID \"" << id << "\" not found." << std::endl;
            return 1;
        }
        return 0;
    }

    else if (argc >= 3 && std::string(argv[1]) == "tag" && std::string(argv[2]) == "list") {
        if (argc < 4) {
            std::cerr << "Error: Missing itinerary ID." << std::endl;
            std::cout << "Usage: " << argv[0] << " tag list <id>" << std::endl;
            return 1;
        }
        listTagsForItinerary(argv[3]);
        return 0;
    }

    else if (argc >= 2 && std::string(argv[1]) == "search") {
        bool hasNameOption = false;
        std::string namePattern;

        for (int i = 2; i < argc - 1; i++) {
            if (std::string(argv[i]) == "--name") {
                hasNameOption = true;
                namePattern = argv[i + 1];
                break;
            }
        }

        if (!hasNameOption) {
            std::cerr << "Error: Missing --name option." << std::endl;
            std::cout << "Usage: travel_planner search --name <pattern>" << std::endl;
            return 1;
        }

        searchItineraries(namePattern);
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
    std::cout << "Usage:" << std::endl;
    std::cout << "  travel_planner [options]" << std::endl;
    std::cout << "  travel_planner <command> [arguments]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help     Display this help message" << std::endl;
    std::cout << "  --version      Display version information" << std::endl;
    std::cout << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  add                   Create a new travel itinerary" << std::endl;
    std::cout << "  list                  List all itineraries" << std::endl;
    std::cout << "  view <id>             View details of a specific itinerary" << std::endl;
    std::cout << "  delete <id>           Delete an itinerary" << std::endl;
    std::cout << "  tag add <id> <tag>    Add a tag to an itinerary" << std::endl;
    std::cout << "  tag remove <id> <tag> Remove a tag from an itinerary" << std::endl;
    std::cout << "  tag list <id>         List all tags for an itinerary" << std::endl;
    std::cout << "  search --name <pattern>  Search for itineraries by name" << std::endl;
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
        "--help", "-h", "--version", "add", "list", "view", "edit", "delete", "--name"
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

void viewItinerary(const std::string& id) {
    // Create storage manager with explicit path
    travel_planner::StorageManager storageManager("data/itineraries.json");

    // Load all itineraries
    std::vector<travel_planner::Itinerary> itineraries = storageManager.loadAll();

    // Find the itinerary with the specified ID
    auto it = std::find_if(itineraries.begin(), itineraries.end(),
        [&id](const travel_planner::Itinerary& itinerary) {
            return itinerary.id == id;
        });

    if (it == itineraries.end()) {
        std::cerr << "Error: No itinerary found with ID '" << id << "'\n";
        return;
    }

    // Display the itinerary details in a formatted way
    const travel_planner::Itinerary& itinerary = *it;

    // Calculate the width for consistent formatting
    size_t labelWidth = 12; // Width for the labels

    std::cout << std::string(50, '=') << '\n';
    std::cout << "ITINERARY DETAILS\n";
    std::cout << std::string(50, '-') << '\n';

    std::cout << std::left << std::setw(labelWidth) << "ID:" << itinerary.id << '\n';
    std::cout << std::left << std::setw(labelWidth) << "Name:" << itinerary.name << '\n';
    std::cout << std::left << std::setw(labelWidth) << "Start Date:" << itinerary.start_date << '\n';
    std::cout << std::left << std::setw(labelWidth) << "End Date:" << itinerary.end_date << '\n';
    std::cout << std::left << std::setw(labelWidth) << "Description:" << '\n';

    // Format description with word wrapping for better readability
    // Simple approach: print the description as is
    std::cout << itinerary.description << '\n';

    std::cout << std::string(50, '=') << '\n';
}

bool deleteItinerary(const std::string& id) {
    // Create storage manager with explicit path
    travel_planner::StorageManager storageManager("data/itineraries.json");

    // Load all itineraries
    std::vector<travel_planner::Itinerary> itineraries = storageManager.loadAll();

    // Find the itinerary with the specified ID
    auto it = std::find_if(itineraries.begin(), itineraries.end(),
        [&id](const travel_planner::Itinerary& itinerary) {
            return itinerary.id == id;
        });

    if (it == itineraries.end()) {
        std::cerr << "Error: No itinerary found with ID '" << id << "'\n";
        return false;
    }

    // Store the name for confirmation message
    std::string name = it->name;

    // Remove the itinerary from the vector
    itineraries.erase(it);

    // Save the updated list back to storage
    storageManager.saveAll(itineraries);

    // Confirmation message
    std::cout << "Successfully deleted itinerary '" << name << "' (ID: " << id << ")\n";
    return true;
}

bool addTagToItinerary(const std::string& id, const std::string& tag, bool& alreadyExists) {
    // Ensure consistent storage path
    const std::string storagePath = "data/itineraries.json";

    travel_planner::StorageManager storageManager(storagePath);
    auto itineraries = storageManager.loadAll();

    // Find itinerary by ID
    auto it = std::find_if(itineraries.begin(), itineraries.end(),
        [&id](const travel_planner::Itinerary& itinerary) {
            return itinerary.id == id;
        });

    if (it == itineraries.end()) {
        return false;
    }

    // Check if tag already exists
    auto& tags = it->tags;
    if (std::find(tags.begin(), tags.end(), tag) != tags.end()) {
        alreadyExists = true;
        return true;
    }

    // Add the tag
    alreadyExists = false;
    tags.push_back(tag);

    // Save changes
    storageManager.saveAll(itineraries);
    return true;
}

bool removeTagFromItinerary(const std::string& id, const std::string& tag, bool& tagExists) {
    // Use EXACTLY the same storage path as in addTagToItinerary
    const std::string storagePath = "data/itineraries.json";

    travel_planner::StorageManager storageManager(storagePath);
    auto itineraries = storageManager.loadAll();

    // Find itinerary by ID - same logic as in addTagToItinerary
    auto it = std::find_if(itineraries.begin(), itineraries.end(),
        [&id](const travel_planner::Itinerary& itinerary) {
            return itinerary.id == id;
        });

    if (it == itineraries.end()) {
        return false; // Itinerary not found
    }

    // Check if tag exists and remove it
    auto& tags = it->tags;
    auto tagIt = std::find(tags.begin(), tags.end(), tag);

    if (tagIt == tags.end()) {
        tagExists = false;
        return true;
    }

    // Remove the tag
    tagExists = true;
    tags.erase(tagIt);

    // Save changes
    storageManager.saveAll(itineraries);
    return true;
}

void listTagsForItinerary(const std::string& id) {
    travel_planner::StorageManager storageManager("data/itineraries.json");
    auto itineraries = storageManager.loadAll();

    auto it = std::find_if(itineraries.begin(), itineraries.end(),
        [&id](const travel_planner::Itinerary& itinerary) {
            return itinerary.id == id;
        });

    if (it == itineraries.end()) {
        std::cerr << "Error: Itinerary with ID '" << id << "' not found." << std::endl;
        return;
    }

    std::cout << "Tags for itinerary '" << it->name << "' (ID: " << it->id << "):" << std::endl;

    if (it->tags.empty()) {
        std::cout << "  No tags found." << std::endl;
        return;
    }

    for (const auto& tag : it->tags) {
        std::cout << "  - " << tag << std::endl;
    }

    std::cout << std::endl << "Total: " << it->tags.size() << " tag(s)" << std::endl;
}

void searchItineraries(const std::string& namePattern) {
    travel_planner::StorageManager storageManager("data/itineraries.json");
    auto itineraries = storageManager.loadAll();

    // Filter itineraries by name pattern (case-insensitive substring match)
    std::vector<travel_planner::Itinerary> matchingItineraries;
    std::string patternLower = namePattern;
    std::transform(patternLower.begin(), patternLower.end(), patternLower.begin(),
        [](unsigned char c) { return std::tolower(c); });

    for (const auto& itinerary : itineraries) {
        std::string nameLower = itinerary.name;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (nameLower.find(patternLower) != std::string::npos) {
            matchingItineraries.push_back(itinerary);
        }
    }

    // Display results in table format (similar to listItineraries)
    if (matchingItineraries.empty()) {
        std::cout << "No itineraries found matching pattern: '" << namePattern << "'" << std::endl;
        return;
    }

    // Calculate column widths
    size_t idColWidth = 2;  // "ID" heading
    size_t nameColWidth = 4;  // "Name" heading

    for (const auto& itinerary : matchingItineraries) {
        idColWidth = std::max(idColWidth, itinerary.id.length());
        nameColWidth = std::max(nameColWidth, itinerary.name.length());
    }

    // Add padding
    idColWidth += 2;
    nameColWidth += 2;

    // Print header
    std::cout << std::string(idColWidth + nameColWidth + 1, '-') << std::endl;
    std::cout << '|' << std::left << std::setw(idColWidth) << " ID"
        << '|' << std::setw(nameColWidth) << " Name" << '|' << std::endl;
    std::cout << std::string(idColWidth + nameColWidth + 1, '-') << std::endl;

    // Print rows
    for (const auto& itinerary : matchingItineraries) {
        std::cout << '|' << std::setw(idColWidth) << " " + itinerary.id
            << '|' << std::setw(nameColWidth) << " " + itinerary.name << '|' << std::endl;
    }

    // Print footer
    std::cout << std::string(idColWidth + nameColWidth + 1, '-') << std::endl;
    std::cout << matchingItineraries.size() << " itinerary/ies found" << std::endl;
}