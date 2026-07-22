#include "JourneyBuilder.h"

#include <algorithm>

#include "DepartureTimeParser.h"

JourneyBuilder::JourneyBuilder(const std::vector<Flight>& flights)
{
    for (const Flight& flight : flights)
    {
        flightsByRoute[flight.route].push_back(flight);
    }

    for (auto& routeFlights : flightsByRoute)
    {
        std::stable_sort(
            routeFlights.second.begin(),
            routeFlights.second.end(),
            [](const Flight& first, const Flight& second)
        {
            return DepartureTimeParser::parse(first.departure)
                < DepartureTimeParser::parse(second.departure);
        });
    }
}

std::vector<Journey> JourneyBuilder::buildJourneys(
    const std::vector<std::vector<Route>>& paths,
    int minimumDepartureTime) const
{
    std::vector<Journey> journeys;

    for (const std::vector<Route>& path : paths)
    {
        std::vector<JourneyLeg> currentLegs;

        buildJourney(
            path,
            0,
            minimumDepartureTime,
            currentLegs,
            journeys);
    }

    std::stable_sort(
        journeys.begin(),
        journeys.end(),
        [](const Journey& first, const Journey& second)
    {
        return first.totalPrice < second.totalPrice;
    });

    return journeys;
}

void JourneyBuilder::buildJourney(
    const std::vector<Route>& path,
    std::size_t routeIndex,
    int earliestDepartureTime,
    std::vector<JourneyLeg>& currentLegs,
    std::vector<Journey>& journeys) const
{
    if (routeIndex == path.size())
    {
        Journey journey;
        journey.legs = currentLegs;
        journey.totalPrice = 0;

        for (const JourneyLeg& leg : currentLegs)
        {
            journey.totalPrice += leg.flight.price;
        }

        journey.totalDuration =
            currentLegs.back().arrivalTime
            - currentLegs.front().departureTime;

        journeys.push_back(std::move(journey));
        return;
    }

    const Route& route = path[routeIndex];

    const auto flightsIterator = flightsByRoute.find(route.routeId);

    if (flightsIterator == flightsByRoute.end())
    {
        return;
    }

    for (const Flight& flight : flightsIterator->second)
    {
        const int departureTime =
            DepartureTimeParser::parse(flight.departure);

        if (departureTime < earliestDepartureTime)
        {
            continue;
        }

        const int arrivalTime =
            departureTime + route.duration;

        JourneyLeg leg;
        leg.route = route;
        leg.flight = flight;
        leg.departureTime = departureTime;
        leg.arrivalTime = arrivalTime;

        currentLegs.push_back(std::move(leg));

        buildJourney(
            path,
            routeIndex + 1,
            arrivalTime,
            currentLegs,
            journeys);

        currentLegs.pop_back();
    }
}
