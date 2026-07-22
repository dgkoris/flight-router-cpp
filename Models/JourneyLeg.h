#pragma once

#include "Route.h"
#include "Flight.h"

class JourneyLeg
{
public:
    Route route;
    Flight flight;

    int departureTime;
    int arrivalTime;
};
