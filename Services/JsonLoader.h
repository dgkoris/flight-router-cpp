#pragma once

#include <string>
#include <vector>

#include "../Models/Flight.h"
#include "../Models/Route.h"

class JsonLoader
{
public:
    std::vector<Route> loadRoutes(const std::string& path) const;
    std::vector<Flight> loadFlights(const std::string& path) const;
};
