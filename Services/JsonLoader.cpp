#include "JsonLoader.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

std::vector<Route> JsonLoader::loadRoutes(const std::string& path) const
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open routes file: " + path);
    }

    nlohmann::json json;
    file >> json;

    std::vector<Route> routes;
    routes.reserve(json.size());

    for (const nlohmann::json& item : json)
    {
        Route route;

        route.routeId = item.at("route_id").get<std::string>();
        route.from = item.at("from").get<std::string>();
        route.to = item.at("to").get<std::string>();
        route.duration = item.at("duration").get<int>();

        routes.push_back(std::move(route));
    }

    return routes;
}

std::vector<Flight> JsonLoader::loadFlights(const std::string& path) const
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open flights file: " + path);
    }

    nlohmann::json json;
    file >> json;

    std::vector<Flight> flights;
    flights.reserve(json.size());

    for (const nlohmann::json& item : json)
    {
        Flight flight;

        flight.flightId = item.at("flight_id").get<int>();
        flight.route = item.at("route").get<std::string>();
        flight.provider = item.at("provider").get<std::string>();
        flight.price = item.at("price").get<int>();
        flight.departure = item.at("departure").get<std::string>();

        flights.push_back(std::move(flight));
    }

    return flights;
}
