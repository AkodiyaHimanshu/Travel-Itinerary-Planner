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