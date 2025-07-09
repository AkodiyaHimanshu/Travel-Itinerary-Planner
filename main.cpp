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
#include <ctime>
#include "src/ExpenseManager.h"
#include "include/version.h"
#include "include/Itinerary.h"
#include "src/StorageManager.h"
#include "src/PackingManager.h"
#include "src/ExportManager.h"


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
void addPackingItem(int argc, char* argv[]);
void listPackingItems(int argc, char* argv[]);
void packItem(int argc, char* argv[]);
void removePackingItem(int argc, char* argv[]);
std::string getCurrentDate();
void addExpense(int argc, char* argv[]);
void listExpenses(int argc, char* argv[]);
void summarizeExpenses(int argc, char* argv[]);
void removeExpense(int argc, char* argv[]);
void exportItinerary(const std::vector<std::string>& args);
void exportPacking(const std::vector<std::string>& args);
void exportExpense(const std::vector<std::string>& args);
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

    else if (argc >= 5 && std::string(argv[1]) == "packing" && std::string(argv[2]) == "add") {
        addPackingItem(argc, argv);
        return 0;
	}

    else if (argc >= 4 && std::string(argv[1]) == "packing" && std::string(argv[2]) == "list") {
        listPackingItems(argc, argv);
        return 0;
	}

    else if (argc >= 4 && std::string(argv[1]) == "packing" && std::string(argv[2]) == "pack") {
        packItem(argc, argv);
        return 0;
	}

    else if (argc >= 4 && std::string(argv[1]) == "packing" && std::string(argv[2]) == "remove") {
        removePackingItem(argc, argv);
		return 0;
	}

    else if (argc >= 5 && std::string(argv[1]) == "expense" && std::string(argv[2]) == "add") {
        addExpense(argc, argv);
        return 0;
	}

    else if (argc >= 4 && std::string(argv[1]) == "expense" && std::string(argv[2]) == "list") {
        listExpenses(argc, argv);
        return 0;
    }

    else if (argc >= 4 && std::string(argv[1]) == "expense" && std::string(argv[2]) == "summary") {
        summarizeExpenses(argc, argv);
        return 0;
	}

    else if (argc >= 4 && std::string(argv[1]) == "expense" && std::string(argv[2]) == "remove") {
        removeExpense(argc, argv);
        return 0;
    }

    else if (argc >= 4 && std::string(argv[1]) == "export" && std::string(argv[2]) == "itinerary") {
        std::vector<std::string> args(argv + 1, argv + argc);
        exportItinerary(args);
        return 0;
	}

    else if (argc >= 4 && std::string(argv[1]) == "export" && std::string(argv[2]) == "packing") {
        std::vector<std::string> args(argv + 1, argv + argc);
        exportPacking(args);
        return 0;
    }

    else if (argc >= 4 && std::string(argv[1]) == "export" && std::string(argv[2]) == "expense") {
        std::vector<std::string> args(argv + 1, argv + argc);
        exportExpense(args);
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
    std::cout << "Usage: travel_planner [COMMAND] [OPTIONS]" << std::endl;
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
    std::cout << "  packing add <itinerary_id> <item_name> [--qty N]  Add a packing item to an itinerary" << std::endl;
    std::cout << "  packing list <itinerary_id>         List all packing items for an itinerary" << std::endl;
    std::cout << "  packing pack <item_id>              Toggle packed status of an item" << std::endl;
    std::cout << "  packing remove <item_id>            Remove an item from the packing list" << std::endl;
    std::cout << "  expense add <itinerary_id> <amount> --category <category> [--date YYYY-MM-DD] [--desc \"<description>\"]" << std::endl;
    std::cout << "      Add a new expense for the specified itinerary" << std::endl;
    std::cout << "  expense list <itinerary_id>" << std::endl;
    std::cout << "      List all expenses for the specified itinerary" << std::endl;
    std::cout << "  travel_planner expense summary <itinerary_id>" << std::endl;
    std::cout << "      Display a summary of expenses by category for the specified itinerary" << std::endl;
    std::cout << "  travel_planner expense remove <expense_id>" << std::endl;
    std::cout << "      Remove an expense by its ID" << std::endl;
    std::cout << "  export itinerary <id> [--format md|csv] Export an itinerary in Markdown (default) or CSV format" << std::endl;
    std::cout << "  export packing <id> [--format md|csv]  Export a packing list to a file (default: Markdown)" << std::endl;
    std::cout << "  export expense <id> [--format md|csv]   Export expenses to a file (default: Markdown)" << std::endl;

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
        "--help", "-h", "--version", "add", "list", "view", "edit", "delete", "--name", "--qty",
        "--category", "--date", "--desc", "--format", "--tag"
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

void addPackingItem(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 5) {
        std::cerr << "Error: Missing required parameters for packing add command." << std::endl;
        std::cerr << "Usage: travel_planner packing add <itinerary_id> <item_name> [--qty N]" << std::endl;
        return;
    }

    std::string itinerary_id = argv[3];
    std::string item_name = argv[4];
    int quantity = 1; // Default quantity

    // Check for optional --qty parameter
    for (int i = 5; i < argc - 1 ; i++) {
        std::string arg = argv[i];
        if (arg == "--qty") {
            try {
                quantity = std::stoi(argv[i + 1]);
                if (quantity <= 0) {
                    std::cerr << "Error: Quantity must be a positive number." << std::endl;
                    return;
                }
            }
            catch (const std::exception&) {
                std::cerr << "Error: Invalid quantity value." << std::endl;
                return;
            }
            break;
        }
    }

    // Create PackingManager and add the item
    travel_planner::PackingManager packingManager("data/packing.json");
    std::string item_id = packingManager.addItem(itinerary_id, item_name, quantity);

    std::cout << "Packing item added with ID: " << item_id << std::endl;
}

void listPackingItems(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 4) {
        std::cerr << "Error: Missing itinerary ID for packing list command." << std::endl;
        std::cerr << "Usage: travel_planner packing list <itinerary_id>" << std::endl;
        return;
    }

    std::string itinerary_id = argv[3];

    // Create PackingManager and load items
    travel_planner::PackingManager packingManager("data/packing.json");
    std::vector<travel_planner::PackingItem> allItems = packingManager.loadAll();

    // Filter items for the specified itinerary
    std::vector<travel_planner::PackingItem> filteredItems;
    for (const auto& item : allItems) {
        if (item.itinerary_id == itinerary_id) {
            filteredItems.push_back(item);
        }
    }

    // Check if there are any items
    if (filteredItems.empty()) {
        std::cout << "No packing items found for itinerary ID: " << itinerary_id << std::endl;
        return;
    }

    // Calculate column widths
    size_t idWidth = 10; // Minimum width
    size_t nameWidth = 20; // Minimum width
    size_t qtyWidth = 8;   // "Quantity"
    size_t statusWidth = 6; // "[Yes]" or "[No]"

    // Find the maximum width needed for each column
    for (const auto& item : filteredItems) {
        idWidth = std::max(idWidth, item.id.length());
        nameWidth = std::max(nameWidth, item.name.length());
    }

    // Add padding
    idWidth += 2;
    nameWidth += 2;

    // Print header
    std::cout << std::left
        << std::setw(idWidth) << "ID"
        << std::setw(nameWidth) << "Item"
        << std::setw(qtyWidth) << "Quantity"
        << "Packed" << std::endl;

    // Print separator
    std::string separator(idWidth + nameWidth + qtyWidth + statusWidth, '-');
    std::cout << separator << std::endl;

    // Print items
    for (const auto& item : filteredItems) {
        std::cout << std::left
            << std::setw(idWidth) << item.id
            << std::setw(nameWidth) << item.name
            << std::setw(qtyWidth) << item.quantity
            << (item.packed ? "[Yes]" : "[No]") << std::endl;
    }

    // Print summary
    std::cout << std::endl;
    std::cout << filteredItems.size() << " item(s) found." << std::endl;
}

void packItem(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 4) {
        std::cerr << "Error: Missing item ID for packing pack command." << std::endl;
        std::cerr << "Usage: travel_planner packing pack <item_id>" << std::endl;
        return;
    }

    std::string item_id = argv[3];

    // Create PackingManager and toggle packed status
    travel_planner::PackingManager packingManager("data/packing.json");

    // Load all items to determine current status
    std::vector<travel_planner::PackingItem> allItems = packingManager.loadAll();
    bool currentStatus = false;
    bool itemFound = false;

    // Find current status of the item
    for (const auto& item : allItems) {
        if (item.id == item_id) {
            currentStatus = item.packed;
            itemFound = true;
            break;
        }
    }

    if (!itemFound) {
        std::cerr << "Error: No item found with ID: " << item_id << std::endl;
        return;
    }

    // Toggle the status
    if (packingManager.markPacked(item_id)) {
        std::cout << "Item " << item_id << " marked as "
            << (currentStatus ? "unpacked" : "packed") << "." << std::endl;
    }
    else {
        std::cerr << "Error: Failed to update item status." << std::endl;
    }
}

void removePackingItem(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 4) {
        std::cerr << "Error: Missing item ID for packing remove command." << std::endl;
        std::cerr << "Usage: travel_planner packing remove <item_id>" << std::endl;
        return;
    }

    std::string item_id = argv[3];

    // Create PackingManager and remove the item
    travel_planner::PackingManager packingManager("data/packing.json");

    if (packingManager.removeItem(item_id)) {
        std::cout << "Item " << item_id << " successfully removed from packing list." << std::endl;
    }
    else {
        std::cerr << "Error: No item found with ID: " << item_id << std::endl;
    }
}

// Add this helper function to get the current date
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d");
    return oss.str();
}

// Add this function to handle the expense add command
void addExpense(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 5) {
        std::cerr << "Error: Missing required parameters for expense add command." << std::endl;
        std::cout << "Usage: " << argv[0] << " expense add <itinerary_id> <amount> --category <category> [--date YYYY-MM-DD] [--desc \"<description>\"]" << std::endl;
        return;
    }

    std::string itinerary_id = argv[3];

    // Parse the amount
    double amount = 0.0;
    try {
        amount = std::stod(argv[4]);
        if (amount <= 0) {
            throw std::invalid_argument("Amount must be greater than zero");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Invalid amount. Please provide a valid positive number." << std::endl;
        return;
    }

    // Check for required --category parameter
    std::string category;
    bool hasCategory = false;

    // Parse optional parameters
    std::string date = getCurrentDate(); // Default to today
    std::string description = ""; // Default to empty

    for (int i = 5; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--category" && i + 1 < argc) {
            category = argv[++i];
            hasCategory = true;
        }
        else if (arg == "--date" && i + 1 < argc) {
            date = argv[++i];
            // Basic date validation could be added here
        }
        else if (arg == "--desc" && i + 1 < argc) {
            description = argv[++i];
        }
    }

    if (!hasCategory) {
        std::cerr << "Error: --category parameter is required." << std::endl;
        return;
    }

    // Add the expense
    travel_planner::ExpenseManager expenseManager("data/expenses.json");
    if (expenseManager.addExpense(itinerary_id, amount, category, date, description)) {
        std::cout << "Expense added successfully." << std::endl;
    }
    else {
        std::cerr << "Error: Failed to add expense." << std::endl;
    }
}

void listExpenses(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 4) {
        std::cerr << "Error: Missing itinerary ID for expense list command." << std::endl;
        std::cout << "Usage: travel_planner expense list <itinerary_id>" << std::endl;
        return;
    }

    std::string itinerary_id = argv[3];

    // Load expenses for the specified itinerary
    travel_planner::ExpenseManager expenseManager("data/expenses.json");
    std::vector<travel_planner::Expense> expenses = expenseManager.listExpenses(itinerary_id);

    // Display the expenses
    std::cout << "Expenses for itinerary: " << itinerary_id << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    if (expenses.empty()) {
        std::cout << "No expenses found for this itinerary." << std::endl;
        return;
    }

    // Find column widths
    size_t id_width = 8;  // Minimum width
    size_t cat_width = 10;  // Minimum width
    size_t date_width = 10;  // Fixed width for date
    size_t desc_width = 15;  // Minimum width

    for (const auto& expense : expenses) {
        id_width = std::max(id_width, expense.id.length());
        cat_width = std::max(cat_width, expense.category.length());
        desc_width = std::max(desc_width, expense.description.length());
    }

    // Cap description width to avoid very long lines
    desc_width = std::min(desc_width, size_t(30));

    // Print header
    std::cout << std::left
        << std::setw(id_width + 2) << "ID"
        << std::setw(12) << "Amount"
        << std::setw(cat_width + 2) << "Category"
        << std::setw(date_width + 2) << "Date"
        << "Description" << std::endl;

    std::cout << std::string(id_width + cat_width + date_width + desc_width + 30, '-') << std::endl;

    // Print each expense
    double total = 0.0;
    for (const auto& expense : expenses) {
        std::cout << std::left
            << std::setw(id_width + 2) << expense.id
            << "$" << std::fixed << std::setprecision(2) << std::setw(10) << expense.amount
            << std::setw(cat_width + 2) << expense.category
            << std::setw(date_width + 2) << expense.date;

        // Handle description that might be too long
        if (expense.description.length() > desc_width) {
            std::cout << expense.description.substr(0, desc_width - 3) << "...";
        }
        else {
            std::cout << expense.description;
        }

        std::cout << std::endl;

        // Sum up the total
        total += expense.amount;
    }

    std::cout << std::string(id_width + cat_width + date_width + desc_width + 30, '-') << std::endl;
    std::cout << "Total: $" << std::fixed << std::setprecision(2) << total << std::endl;
    std::cout << expenses.size() << " expense(s) found." << std::endl;
}

void summarizeExpenses(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 4) {
        std::cerr << "Error: Missing itinerary ID for expense summary command." << std::endl;
        std::cout << "Usage: travel_planner expense summary <itinerary_id>" << std::endl;
        return;
    }

    std::string itinerary_id = argv[3];

    // Get expense summary by category
    travel_planner::ExpenseManager expenseManager("data/expenses.json");
    std::map<std::string, double> categorySummary = expenseManager.summary(itinerary_id);

    // Display the summary
    std::cout << "Expense Summary for itinerary: " << itinerary_id << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    if (categorySummary.empty()) {
        std::cout << "No expenses found for this itinerary." << std::endl;
        return;
    }

    // Find maximum category name length for formatting
    size_t categoryWidth = 10;  // Minimum width
    for (const auto& entry : categorySummary) {
        categoryWidth = std::max(categoryWidth, entry.first.length());
    }

    // Print header
    std::cout << std::left
        << std::setw(categoryWidth + 5) << "Category"
        << "Amount" << std::endl;

    std::cout << std::string(categoryWidth + 20, '-') << std::endl;

    // Print each category with total
    double overallTotal = 0.0;
    for (const auto& entry : categorySummary) {
        std::cout << std::left
            << std::setw(categoryWidth + 5) << entry.first
            << "$" << std::fixed << std::setprecision(2) << entry.second << std::endl;

        // Add to overall total
        overallTotal += entry.second;
    }

    // Print overall total
    std::cout << std::string(categoryWidth + 20, '-') << std::endl;
    std::cout << std::left
        << std::setw(categoryWidth + 5) << "TOTAL"
        << "$" << std::fixed << std::setprecision(2) << overallTotal << std::endl;

    std::cout << std::endl << categorySummary.size() << " category/categories found." << std::endl;
}

void removeExpense(int argc, char* argv[]) {
    // Check for required parameters
    if (argc < 4) {
        std::cerr << "Error: Missing expense ID for expense remove command." << std::endl;
        std::cout << "Usage: travel_planner expense remove <expense_id>" << std::endl;
        return;
    }

    std::string expense_id = argv[3];

    // Attempt to remove the expense
    travel_planner::ExpenseManager expenseManager("data/expenses.json");
    bool success = expenseManager.removeExpense(expense_id);

    if (success) {
        std::cout << "Expense with ID '" << expense_id << "' was successfully removed." << std::endl;
    }
    else {
        std::cerr << "Error: Failed to remove expense. Expense with ID '" << expense_id << "' not found." << std::endl;
    }
}

// Helper function to handle common export operations
bool doExport(const std::string& what,
    const std::string& id,
    const std::string& format,
    std::function<bool(const std::string&, const std::string&)> mdFn,
    std::function<bool(const std::string&, const std::string&)> csvFn) {

    travel_planner::ExportManager exportManager;
    std::string path = "exports/" + what + "_" + id + "." + format;

    if (format == "md") {
        return mdFn(id, path);
    }
    else {
        return csvFn(id, path);
    }
}

// Simplified exportItinerary function
void exportItinerary(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cerr << "Error: Missing itinerary ID for export" << std::endl;
        std::cout << "Usage: travel_planner export itinerary <id> [--format md|csv]" << std::endl;
        return;
    }

    std::string itineraryId = args[2];
    std::string format = "md"; // Default to markdown

    // Check for format flag
    for (size_t i = 3; i < args.size(); i++) {
        if (args[i] == "--format" && i + 1 < args.size()) {
            format = args[i + 1];
            if (format != "md" && format != "csv") {
                std::cerr << "Error: Invalid format specified. Use 'md' or 'csv'." << std::endl;
                return;
            }
            break;
        }
    }

    travel_planner::ExportManager exportManager;
    bool success = doExport(
        "itinerary",
        itineraryId,
        format,
        [&exportManager](const std::string& id, const std::string& path) {
            return exportManager.exportItineraryMarkdown(id, path);
        },
        [&exportManager](const std::string& id, const std::string& path) {
            return exportManager.exportItineraryCSV(id, path);
        }
    );

    if (success) {
        std::cout << "Itinerary " << itineraryId << " exported successfully as "
            << format << " format." << std::endl;
    }
    else {
        std::cerr << "Error: Failed to export itinerary. Please check if the itinerary exists." << std::endl;
    }
}

// Simplified exportPacking function
void exportPacking(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cerr << "Error: Missing itinerary ID for packing list export" << std::endl;
        std::cout << "Usage: travel_planner export packing <itinerary_id> [--format md|csv]" << std::endl;
        return;
    }

    std::string itineraryId = args[2];
    std::string format = "md"; // Default to markdown

    // Check for format flag
    for (size_t i = 3; i < args.size(); i++) {
        if (args[i] == "--format" && i + 1 < args.size()) {
            format = args[i + 1];
            if (format != "md" && format != "csv") {
                std::cerr << "Error: Invalid format specified. Use 'md' or 'csv'." << std::endl;
                return;
            }
            break;
        }
    }

    travel_planner::ExportManager exportManager;
    bool success = doExport(
        "packing",
        itineraryId,
        format,
        [&exportManager](const std::string& id, const std::string& path) {
            return exportManager.exportPackingMarkdown(id, path);
        },
        [&exportManager](const std::string& id, const std::string& path) {
            return exportManager.exportPackingCSV(id, path);
        }
    );

    if (success) {
        std::cout << "Packing list for itinerary " << itineraryId << " exported successfully as "
            << format << " format." << std::endl;
    }
    else {
        std::cerr << "Error: Failed to export packing list. Please check if the itinerary exists." << std::endl;
    }
}

// Simplified exportExpense function
void exportExpense(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cerr << "Error: Missing itinerary ID for expense export" << std::endl;
        std::cout << "Usage: travel_planner export expense <itinerary_id> [--format md|csv]" << std::endl;
        return;
    }

    std::string itineraryId = args[2];
    std::string format = "md"; // Default to markdown

    // Check for format flag
    for (size_t i = 3; i < args.size(); i++) {
        if (args[i] == "--format" && i + 1 < args.size()) {
            format = args[i + 1];
            if (format != "md" && format != "csv") {
                std::cerr << "Error: Invalid format specified. Use 'md' or 'csv'." << std::endl;
                return;
            }
            break;
        }
    }

    travel_planner::ExportManager exportManager;
    bool success = doExport(
        "expenses",
        itineraryId,
        format,
        [&exportManager](const std::string& id, const std::string& path) {
            return exportManager.exportExpenseMarkdown(id, path);
        },
        [&exportManager](const std::string& id, const std::string& path) {
            return exportManager.exportExpenseCSV(id, path);
        }
    );

    if (success) {
        std::cout << "Expenses for itinerary " << itineraryId << " exported successfully as "
            << format << " format." << std::endl;
    }
    else {
        std::cerr << "Error: Failed to export expenses. Please check if the itinerary exists." << std::endl;
    }
void favoriteItinerary(const std::string& id) {
    if (id.empty()) {
        std::cerr << "Error: Itinerary ID is required." << std::endl;
        return;
    }

    travel_planner::StorageManager storageManager("data/itineraries.json");
    std::vector<travel_planner::Itinerary> itineraries = storageManager.loadAll();

    auto it = std::find_if(itineraries.begin(), itineraries.end(),
        [&id](const travel_planner::Itinerary& itinerary) {
            return itinerary.id == id;
        });

    if (it == itineraries.end()) {
        std::cerr << "Error: No itinerary found with ID: " << id << std::endl;
        return;
    }

    // Set the itinerary as favorite
    it->is_favorite = true;

    // Save the updated list
    storageManager.saveAll(itineraries);
    std::cout << "Itinerary '" << it->name << "' marked as favorite." << std::endl;
}
void unfavoriteItinerary(const std::string& id) {
    if (id.empty()) {
        std::cerr << "Error: Itinerary ID is required." << std::endl;
        return;
    }

    travel_planner::StorageManager storageManager("data/itineraries.json");
    std::vector<travel_planner::Itinerary> itineraries = storageManager.loadAll();

    auto it = std::find_if(itineraries.begin(), itineraries.end(),
        [&id](const travel_planner::Itinerary& itinerary) {
            return itinerary.id == id;
        });

    if (it == itineraries.end()) {
        std::cerr << "Error: No itinerary found with ID: " << id << std::endl;
        return;
    }

    // Remove the favorite status
    it->is_favorite = false;

    // Save the updated list
    storageManager.saveAll(itineraries);
    std::cout << "Favorite status removed from itinerary '" << it->name << "'." << std::endl;
}
void listFavoriteItineraries() {
    travel_planner::StorageManager storageManager("data/itineraries.json");
    std::vector<travel_planner::Itinerary> allItineraries = storageManager.loadAll();

    // Filter for favorite itineraries only
    std::vector<travel_planner::Itinerary> favoriteItineraries;
    std::copy_if(allItineraries.begin(), allItineraries.end(),
        std::back_inserter(favoriteItineraries),
        [](const travel_planner::Itinerary& itin) { return itin.is_favorite; });

    if (favoriteItineraries.empty()) {
        std::cout << "No favorite itineraries found." << std::endl;
        return;
    }

    // Find the longest ID and name for formatting
    size_t maxIdLength = 2; // "ID" header length
    size_t maxNameLength = 4; // "Name" header length

    for (const auto& itinerary : favoriteItineraries) {
        maxIdLength = std::max(maxIdLength, itinerary.id.length());
        maxNameLength = std::max(maxNameLength, itinerary.name.length());
    }

    // Add some padding
    maxIdLength += 2;
    maxNameLength += 2;

    // Print headers
    std::cout << std::left << std::setw(maxIdLength) << "ID"
        << std::setw(maxNameLength) << "Name" << std::endl;

    // Print separator line
    std::cout << std::string(maxIdLength + maxNameLength, '-') << std::endl;

    // Print itineraries
    for (const auto& itinerary : favoriteItineraries) {
        std::cout << std::left << std::setw(maxIdLength) << itinerary.id
            << std::setw(maxNameLength) << itinerary.name << std::endl;
    }

    // Print count
    std::cout << std::endl << favoriteItineraries.size()
        << " favorite " << (favoriteItineraries.size() == 1 ? "itinerary" : "itineraries")
        << " found." << std::endl;
}
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Helper function for case-insensitive substring check
bool caseInsensitiveContains(const std::string& str, const std::string& substr) {
    std::string lowerStr = toLower(str);
    std::string lowerSubstr = toLower(substr);
    return lowerStr.find(lowerSubstr) != std::string::npos;
}

// Function to search itineraries by keyword
void searchItinerariesByKeyword(const std::string& keyword) {
    if (keyword.empty()) {
        std::cerr << "Error: Search keyword is required." << std::endl;
        return;
    }

    travel_planner::StorageManager storageManager("data/itineraries.json");
    std::vector<travel_planner::Itinerary> allItineraries = storageManager.loadAll();

    // Filter for matching itineraries
    std::vector<travel_planner::Itinerary> matchingItineraries;

    for (const auto& itinerary : allItineraries) {
        if (caseInsensitiveContains(itinerary.name, keyword) ||
            caseInsensitiveContains(itinerary.description, keyword)) {
            matchingItineraries.push_back(itinerary);
        }
    }

    if (matchingItineraries.empty()) {
        std::cout << "No itineraries found matching '" << keyword << "'." << std::endl;
        return;
    }

    // Find the longest ID and name for formatting
    size_t maxIdLength = 2; // "ID" header length
    size_t maxNameLength = 4; // "Name" header length

    for (const auto& itinerary : matchingItineraries) {
        maxIdLength = std::max(maxIdLength, itinerary.id.length());
        maxNameLength = std::max(maxNameLength, itinerary.name.length());
    }

    // Add some padding
    maxIdLength += 2;
    maxNameLength += 2;

    // Print headers
    std::cout << std::left << std::setw(maxIdLength) << "ID"
        << std::setw(maxNameLength) << "Name" << std::endl;

    // Print separator line
    std::cout << std::string(maxIdLength + maxNameLength, '-') << std::endl;

    // Print matching itineraries
    for (const auto& itinerary : matchingItineraries) {
        std::cout << std::left << std::setw(maxIdLength) << itinerary.id
            << std::setw(maxNameLength) << itinerary.name << std::endl;
    }

    // Print count
    std::cout << std::endl << matchingItineraries.size()
        << " " << (matchingItineraries.size() == 1 ? "itinerary" : "itineraries")
        << " found matching '" << keyword << "'." << std::endl;
}