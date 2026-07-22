#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "../Models/Flight.h"
#include "../Models/Journey.h"
#include "../Models/JourneyLeg.h"
#include "../Models/Route.h"

class JourneyBuilder
{
public:
    explicit JourneyBuilder(const std::vector<Flight>& flights);

    std::vector<Journey> buildJourneys(
        const std::vector<std::vector<Route>>& paths,
        int minimumDepartureTime) const;

private:
    std::unordered_map<std::string, std::vector<Flight>> flightsByRoute;

    void buildJourney(
        const std::vector<Route>& path,
        std::size_t routeIndex,
        int earliestDepartureTime,
        std::vector<JourneyLeg>& currentLegs,
        std::vector<Journey>& journeys) const;
};
