#include <iostream>
#include <vector>

#include "Models/Flight.h"
#include "Models/Route.h"
#include "Services/JsonLoader.h"

int main()
{
    try
    {
        JsonLoader loader;

        const std::vector<Route> routes = loader.loadRoutes("Data/Routes.json");

        const std::vector<Flight> flights = loader.loadFlights("Data/Flights.json");

        std::cout << "Loaded " << routes.size() << " routes.\n";
        std::cout << "Loaded " << flights.size() << " flights.\n\n";

        if (!routes.empty())
        {
            const Route& route = routes.front();

            std::cout << "First route\n";
            std::cout << "-------------\n";
            std::cout << "Route ID : " << route.routeId << '\n';
            std::cout << "From     : " << route.from << '\n';
            std::cout << "To       : " << route.to << '\n';
            std::cout << "Duration : " << route.duration << " hours\n\n";
        }

        if (!flights.empty())
        {
            const Flight& flight = flights.front();

            std::cout << "First flight\n";
            std::cout << "--------------\n";
            std::cout << "Flight ID : " << flight.flightId << '\n';
            std::cout << "Route     : " << flight.route << '\n';
            std::cout << "Provider  : " << flight.provider << '\n';
            std::cout << "Price     : " << flight.price << '\n';
            std::cout << "Departure : " << flight.departure << '\n';
        }
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';
        return 1;
    }

    return 0;
}
