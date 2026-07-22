#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Models/Flight.h"
#include "Models/Journey.h"
#include "Models/JourneyLeg.h"
#include "Models/Route.h"
#include "Services/DepartureTimeParser.h"
#include "Services/JsonLoader.h"
#include "Services/JourneyBuilder.h"
#include "Services/PathFinder.h"

std::string normaliseInput(std::string input)
{
    input.erase(
        std::remove_if(
            input.begin(),
            input.end(),
            [](unsigned char character)
    {
        return std::isspace(character);
    }),
        input.end());

    std::transform(
        input.begin(),
        input.end(),
        input.begin(),
        [](unsigned char character)
    {
        return static_cast<char>(std::toupper(character));
    });

    return input;
}

std::string formatTime(int totalHours)
{
    const int days = totalHours / 24;
    const int hours = totalHours % 24;

    return "+"
        + std::to_string(days)
        + "d "
        + std::to_string(hours)
        + "h";
}

void printJourney(const Journey& journey)
{
    for (std::size_t index = 0; index < journey.legs.size(); index++)
    {
        const JourneyLeg& leg = journey.legs[index];

        if (index > 0)
        {
            std::cout << " | ";
        }

        std::cout
            << leg.route.from
            << "->"
            << leg.route.to
            << " ("
            << leg.flight.provider
            << ", GBP "
            << leg.flight.price
            << ", "
            << formatTime(leg.departureTime)
            << "-"
            << formatTime(leg.arrivalTime)
            << ")";
    }

    std::cout
        << " | Total: GBP "
        << journey.totalPrice
        << " | Duration: "
        << journey.totalDuration
        << " hours\n";
}

int main()
{
    try
    {
        const std::string defaultDeparture = "+0 day 0 hour";
        const std::string defaultJourneyType = "1";

        JsonLoader loader;

        const std::vector<Route> routes = loader.loadRoutes("Data/Routes.json");

        const std::vector<Flight> flights = loader.loadFlights("Data/Flights.json");

        PathFinder pathFinder(routes);
        JourneyBuilder journeyBuilder(flights);

        std::string origin;
        std::string destination;
        std::string minimumDeparture;
        std::string journeyType;

        std::cout << "Origin: ";
        std::getline(std::cin, origin);

        std::cout << "Destination: ";
        std::getline(std::cin, destination);

        std::cout
            << "Minimum departure (press Enter for default: "
            << defaultDeparture
            << "): ";
        std::getline(std::cin, minimumDeparture);

        std::cout
            << "Journey type (1 = Cheapest, 2 = Shortest) "
            << "(press Enter for default: 1): ";
        std::getline(std::cin, journeyType);

        if (minimumDeparture.empty())
        {
            minimumDeparture = defaultDeparture;
        }

        if (journeyType.empty())
        {
            journeyType = defaultJourneyType;
        }

        origin = normaliseInput(origin);
        destination = normaliseInput(destination);
        journeyType = normaliseInput(journeyType);

        if (origin.empty() || destination.empty())
        {
            std::cerr
                << "Please enter an origin and destination.\n";

            return 1;
        }

        if (origin == destination)
        {
            std::cerr
                << "Origin and destination must be different.\n";

            return 1;
        }

        if (journeyType != "1" && journeyType != "2")
        {
            std::cerr
                << "Journey type must be 1 or 2.\n";

            return 1;
        }

        const int minimumDepartureTime =
            DepartureTimeParser::parse(minimumDeparture);

        const std::vector<std::vector<Route>> paths =
            pathFinder.findAllPaths(origin, destination);

        if (paths.empty())
        {
            std::cout
                << "No path exists from "
                << origin
                << " to "
                << destination
                << ".\n";

            return 0;
        }

        std::vector<Journey> journeys =
            journeyBuilder.buildJourneys(
                paths,
                minimumDepartureTime);

        if (journeys.empty())
        {
            std::cout
                << "No valid journeys exist from "
                << origin
                << " to "
                << destination
                << ".\n";

            return 0;
        }

        if (journeyType == "2")
        {
            std::stable_sort(
                journeys.begin(),
                journeys.end(),
                [](const Journey& first, const Journey& second)
            {
                if (first.totalDuration != second.totalDuration)
                {
                    return first.totalDuration
                        < second.totalDuration;
                }

                return first.totalPrice < second.totalPrice;
            });
        }

        const std::string order =
            journeyType == "2" ? "duration" : "price";

        std::cout
            << "\nFound "
            << journeys.size()
            << " valid journeys from "
            << origin
            << " to "
            << destination
            << ", ordered by "
            << order
            << ".\n\n";

        for (const Journey& journey : journeys)
        {
            printJourney(journey);
        }
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';
        return 1;
    }

    return 0;
}
