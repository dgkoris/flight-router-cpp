#pragma once

#include <vector>

#include "JourneyLeg.h"

class Journey
{
public:
    std::vector<JourneyLeg> legs;

    int totalPrice;
    int totalDuration;
};
