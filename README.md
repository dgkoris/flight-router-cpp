# Flight Router (C++)

## Requirements

- Visual Studio with C++ Desktop Development support
- C++17

## Running

1. Open the solution in Visual Studio.
2. Build the solution.
3. Run the application.

## Assumptions & Notes

- The C++ implementation was completed after the C# version and uses the same corrected JSON data.
- There is no minimum waiting time between connecting flights.
- Departure times are interpreted as hours relative to the current time (for example `+2 day 5 hour`).
- Locations are not revisited within the same path to avoid cycles.
- The same input data is used as the C# version.

## Solution

The application is organised into small components:

- `JsonLoader` loads the input data.
- `PathFinder` finds all possible paths between two locations.
- `JourneyBuilder` builds valid journeys using the available flights.
- `DepartureTimeParser` parses the relative departure time strings.

Task 1 uses depth-first search (DFS) to find all valid paths.

Tasks 2 and 3 build valid journeys while checking flight timings. Results can be ordered by total price or total journey duration.
