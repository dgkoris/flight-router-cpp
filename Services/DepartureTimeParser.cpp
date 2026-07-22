#include "DepartureTimeParser.h"

#include <regex>
#include <stdexcept>

int DepartureTimeParser::parse(const std::string& departure)
{
    if (departure.empty())
    {
        throw std::invalid_argument("Departure time cannot be empty.");
    }

    static const std::regex departurePattern(
        R"(^\+(\d+)\s+day\s+(\d+)\s+hour$)",
        std::regex_constants::icase);

    std::smatch match;

    if (!std::regex_match(departure, match, departurePattern))
    {
        throw std::invalid_argument(
            "Invalid departure time format: " + departure);
    }

    const int days = std::stoi(match[1].str());
    const int hours = std::stoi(match[2].str());

    return days * 24 + hours;
}
