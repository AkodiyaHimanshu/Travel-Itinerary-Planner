// Travel Itinerary Planner.cpp : Defines the entry point for the application.
//

#include "main.h"

using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "include/version.h" // Include the version header

void displayBanner() {
    const std::string banner = "Travel Itinerary Planner CLI";
    const std::string separator(banner.length(), '=');

    std::cout << separator << std::endl
        << banner << std::endl
        << separator << std::endl;
}

void displayHelp() {
    std::cout << "Usage: travel_planner [OPTIONS]\n"
        << "Options:\n"
        << "  -h, --help     Display this help message\n"
        << "  --version      Display version information\n"
        << "  --create       Create a new itinerary\n"
        << "  --list         List all itineraries\n"
        << "  --view ID      View itinerary with specified ID\n"
        << "  --edit ID      Edit itinerary with specified ID\n"
        << "  --delete ID    Delete itinerary with specified ID\n";
}

void displayVersion() {
    std::cout << "Travel Itinerary Planner v" << TRAVEL_PLANNER_VERSION << std::endl
        << "Build date: " << TRAVEL_PLANNER_BUILD_DATE << std::endl;
}

bool hasOption(const std::vector<std::string>& args, const std::string& option) {
    return std::find(args.begin(), args.end(), option) != args.end();
}



int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty()) {
        displayBanner();
        displayHelp();
        return 0;
    }

    // Check for unknown options
    std::string unknownOption = findUnknownOption(args);
    if (!unknownOption.empty()) {
            std::cerr << "Error: Unknown option: " << unknownOption << std::endl;
            displayHelp();
            return 1;
    }

    if (hasOption(args, "--help") || hasOption(args, "-h")) {
        displayBanner();
        displayHelp();
        return 0;
    }

    if (hasOption(args, "--version")) {
        displayVersion();
        return 0;
    }

    displayBanner();
    // Main application logic will go here

    return 0;
}