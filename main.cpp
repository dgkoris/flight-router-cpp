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

std::string normaliseLocation(std::string location)
{
    location.erase(
        std::remove_if(
            location.begin(),
            location.end(),
            [](unsigned char character)
    {
        return std::isspace(character);
    }),
        location.end());

    std::transform(
        location.begin(),
        location.end(),
        location.begin(),
        [](unsigned char character)
    {
        return static_cast<char>(std::toupper(character));
    });

    return location;
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

        JsonLoader loader;

        const std::vector<Route> routes = loader.loadRoutes("Data/Routes.json");

        const std::vector<Flight> flights = loader.loadFlights("Data/Flights.json");

        PathFinder pathFinder(routes);
        JourneyBuilder journeyBuilder(flights);

        std::string origin;
        std::string destination;
        std::string minimumDeparture;

        std::cout << "Origin: ";
        std::getline(std::cin, origin);

        std::cout << "Destination: ";
        std::getline(std::cin, destination);

        std::cout
            << "Minimum departure (press Enter for default: "
            << defaultDeparture
            << "): ";
        std::getline(std::cin, minimumDeparture);

        if (minimumDeparture.empty())
        {
            minimumDeparture = defaultDeparture;
        }

        origin = normaliseLocation(origin);
        destination = normaliseLocation(destination);

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

        const std::vector<Journey> journeys =
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

        std::cout
            << "\nFound "
            << journeys.size()
            << " valid journeys from "
            << origin
            << " to "
            << destination
            << ", ordered by price.\n\n";

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
