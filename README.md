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

## Packing List Management

The Travel Itinerary Planner includes comprehensive packing list management to help you organize what to bring on your trips.

### Adding Packing Items

Add items to your packing list by specifying an itinerary ID and item name. Optionally, you can specify a quantity:

```bash
# Add a single item
$ travel_planner packing add trip-123 "Toothbrush"
Packing item added with ID: 1683559421-a7b3f9

# Add multiple of the same item
$ travel_planner packing add trip-123 "T-shirts" --qty 3
Packing item added with ID: 1683559589-c4d2e8
```

### Listing Packing Items

View all packing items for a specific itinerary:

```bash
$ travel_planner packing list trip-123
ID                  Item                  Quantity  Packed
----------------------------------------------------
1683559421-a7b3f9   Toothbrush            1         [No]
1683559589-c4d2e8   T-shirts              3         [No]

2 item(s) found.
```

### Marking Items as Packed/Unpacked

Toggle the packed status of an item using its ID:

```bash
# Mark an item as packed
$ travel_planner packing pack 1683559421-a7b3f9
Item 1683559421-a7b3f9 marked as packed.

# Mark it as unpacked (running the command again toggles the status)
$ travel_planner packing pack 1683559421-a7b3f9
Item 1683559421-a7b3f9 marked as unpacked.
```

### Removing Packing Items

Remove an item from your packing list:

```bash
$ travel_planner packing remove 1683559589-c4d2e8
Item 1683559589-c4d2e8 successfully removed from packing list.
```

### Typical Workflow Example

```bash
# 1. First, create an itinerary
$ travel_planner add

# 2. Add packing items for the itinerary
$ travel_planner packing add abc-123 "Passport"
$ travel_planner packing add abc-123 "Underwear" --qty 5
$ travel_planner packing add abc-123 "Phone charger"

# 3. List all packing items to see what needs to be packed
$ travel_planner packing list abc-123

# 4. Mark items as packed as you pack them
$ travel_planner packing pack 1683559700-f5g6h7

# 5. Check your packing progress
$ travel_planner packing list abc-123

# 6. Remove items you decide not to pack
$ travel_planner packing remove 1683559800-i7j8k9
```

This section provides a comprehensive overview of the packing list functionality, with clear examples for each command and their expected outputs. The formatting follows the existing README style with bash syntax highlighting for the code blocks.

## Managing Expenses

The Travel Itinerary Planner allows you to track and manage expenses for your trips. Below are examples of the expense management commands.

### Adding an Expense

Add an expense to an itinerary with an amount and category. Date and description are optional.

```bash
$ travel_planner expense add trip-123 125.50 --category "Transportation" --date 2023-07-15 --desc "Airport taxi"
Expense added successfully.
```

Basic usage (uses current date and empty description):

```bash
$ travel_planner expense add trip-123 45.75 --category "Food"
Expense added successfully.
```

### Listing Expenses

View all expenses for a specific itinerary:

```bash
$ travel_planner expense list trip-123
Expenses for itinerary: trip-123

ID                  Amount     Category       Date        Description
-----------------------------------------------------------------------
1626384059-a3b4c5   $125.50    Transportation 2023-07-15  Airport taxi
1626384128-d5e6f7   $45.75     Food           2023-07-16
1626384240-g7h8i9   $200.00    Accommodation  2023-07-16  Hotel

Total: $371.25
3 expense(s) found.
```

### Expense Summary by Category

Get a summary of expenses grouped by category:

```bash
$ travel_planner expense summary trip-123
Expense Summary for itinerary: trip-123

Category        Amount
-----------------------
Accommodation   $200.00
Food            $45.75
Transportation  $125.50

TOTAL           $371.25

3 category/categories found.
```

### Removing an Expense

Remove an expense using its ID:

```bash
$ travel_planner expense remove 1626384059-a3b4c5
Expense with ID '1626384059-a3b4c5' was successfully removed.
```

If the expense ID doesn't exist:

```bash
$ travel_planner expense remove invalid-id
Error: Failed to remove expense. Expense with ID 'invalid-id' not found.
```


## License

[Your chosen license]
