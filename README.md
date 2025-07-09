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

## Exporting Data

The Travel Itinerary Planner allows you to export your data in different formats for sharing, printing, or analysis purposes.

### Export Commands

All export commands follow the same pattern:
```
travel_planner export <type> <itinerary_id> [--format <format>]
```

Where:
- `<type>` is one of: `itinerary`, `packing`, or `expense`
- `<itinerary_id>` is the ID of the itinerary to export
- `<format>` is either `md` (Markdown) or `csv` (Comma Separated Values)

If no format is specified, Markdown (md) is used by default.

### Export Itineraries

Export an itinerary's details to a file:
```
travel_planner export itinerary ABC123 --format md
```

**Sample Markdown output:**
```
Itinerary: Summer Vacation in Europe
ID: ABC123
Start Date: 2023-06-15
End Date: 2023-06-30
Description: A two-week tour of major European cities including Paris, Rome, and Barcelona.
Created on: June 1, 2023
```

**Sample CSV output:**
```
ID,Name,Start Date,End Date,Description
ABC123,Summer Vacation in Europe,2023-06-15,2023-06-30,"A two-week tour of major European cities including Paris, Rome, and Barcelona."
```

### Export Packing Lists

Export a packing list for a specific itinerary:
```
travel_planner export packing ABC123 --format md
```

**Sample Markdown output:**
```
Packing List for: Summer Vacation in Europe
Packed Items
 Passport
 Travel Insurance Documents
 Phone Charger
Unpacked Items
 Sunscreen
 Beach Towel
 Swimsuit
```

**Sample CSV output:**
```
ID,Name,Packed,Category,Quantity,Itinerary ID
PKI001,Passport,true,Documents,1,ABC123
PKI002,Travel Insurance Documents,true,Documents,1,ABC123
PKI003,Phone Charger,true,Electronics,1,ABC123
PKI004,Sunscreen,false,Toiletries,1,ABC123
PKI005,Beach Towel,false,Clothing,1,ABC123
PKI006,Swimsuit,false,Clothing,1,ABC123
```

### Export Expenses

Export expenses for a specific itinerary:
```
travel_planner export expense ABC123 --format md
```

**Sample Markdown output:**
```
Expenses for: Summer Vacation in Europe
Date	Category	Description	Amount
2023-06-15	Transportation	Flight to Paris	$450.00
2023-06-16	Accommodation	Hotel in Paris	$180.00
2023-06-17	Food	Dinner at Le Bistro	$65.75
Total: $695.75
```

**Sample CSV output:**
```
ID,Date,Category,Description,Amount,Itinerary ID
EXP001,2023-06-15,Transportation,Flight to Paris,450.00,ABC123
EXP002,2023-06-16,Accommodation,Hotel in Paris,180.00,ABC123
EXP003,2023-06-17,Food,Dinner at Le Bistro,65.75,ABC123
```

### Export File Location

All exported files are saved in the `exports/` directory with filenames based on the export type and itinerary ID:

- Itineraries: `exports/itinerary_<id>.[md|csv]`
- Packing Lists: `exports/packing_<id>.[md|csv]`
- Expenses: `exports/expenses_<id>.[md|csv]`


## Managing Favorites

You can mark itineraries as favorites for easy access and reference.

### Marking Favorites

To mark an itinerary as a favorite:
```
./travel_planner itinerary favorite <id>
```

or use the shortcut:
```
./travel_planner fav <id>
```

Example:
```
./travel_planner fav abc123
```

Output:
```
Itinerary 'Summer in Paris' marked as favorite.
```

### Removing Favorite Status

To remove the favorite status:
```
./travel_planner itinerary unfavorite <id>
```

or use the shortcut:
```
./travel_planner unfav <id>
```

Example:
```
./travel_planner unfav abc123
```

Output:
```
Favorite status removed from itinerary 'Summer in Paris'.
```

### Viewing Favorite Itineraries

To view only your favorite itineraries:
```
./travel_planner itinerary favorites
```

Example output:
```
ID      Name                  Favorite
abc123  Summer in Paris       ★
def456  Tokyo Adventure       ★

2 itineraries found.
```

### Visual Indication

When listing all itineraries, favorite items are marked with a star (★) symbol:
```
./travel_planner list
```

Example output:
```
ID      Name                  Favorite
abc123  Summer in Paris       ★
def456  Tokyo Adventure       ★
ghi789  London Weekend

3 itineraries found.
```

## Searching Itineraries

You can search through your itineraries using keywords that match against the name or description:
```
./travel_planner itinerary search <keyword>
```

Example:
```
./travel_planner itinerary search beach
```

Output:
```
ID      Name                  Favorite
jkl012  Beach Getaway         ★
mno345  Miami Spring Break

2 itineraries found matching 'beach'.
```

The search is case-insensitive and will match partial words in both the name and description fields.


## License

[Your chosen license]
