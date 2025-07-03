# Travel Itinerary Planner

A command-line application for creating and managing travel itineraries.

## Requirements

- C++17 compatible compiler  
- CMake 3.14 or higher  
- Git

## Building from Source

Clone the repository:
```bash
git clone https://github.com/yourusername/travel-itinerary-planner.git  
cd travel-itinerary-planner
```

Build with CMake:
```bash
mkdir build && cd build  
cmake ..  
make
```

## Running the Application

After building, you can run the application from the build directory:
```bash
./travel_planner
```

## Command-Line Options

Show help information:
```bash
./travel_planner --help
```
or
```bash
./travel_planner -h
```

Display version information:
```bash
./travel_planner --version
```

Create a new itinerary:
```bash
./travel_planner add
```

List all itineraries:
```bash
./travel_planner list
```

View a specific itinerary:
```bash
./travel_planner view <itinerary_id>
```

Delete an itinerary:
```bash
./travel_planner delete <itinerary_id>
```

# Working with Tags

Add a tag to an itinerary:
```bash
./travel_planner tag add <itinerary_id> vacation
```

Add multiple tags (run commands sequentially):
```bash
./travel_planner tag add <itinerary_id> family
./travel_planner tag add <itinerary_id> budget
```

List all tags for an itinerary:
```bash
./travel_planner tag list <itinerary_id>
```

Remove a tag:
```bash
./travel_planner tag remove <itinerary_id> budget
```

# Searching and Filtering

Search itineraries by name (case-insensitive substring match):
```bash
./travel_planner search --name vacation
```

Search for itineraries with partial name match:
```bash
./travel_planner search --name euro
Will find itineraries with names like "Europe Trip", "Eurostar Journey", etc.
```

## License

[Your chosen license]
