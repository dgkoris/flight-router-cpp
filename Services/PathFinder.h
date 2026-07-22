#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Models/Route.h"

class PathFinder
{
public:
    explicit PathFinder(const std::vector<Route>& routes);

    std::vector<std::vector<Route>> findAllPaths(
        const std::string& origin,
        const std::string& destination) const;

private:
    std::unordered_map<std::string, std::vector<Route>> routesByOrigin;

    void findPaths(
        const std::string& current,
        const std::string& destination,
        std::unordered_set<std::string>& visited,
        std::vector<Route>& currentPath,
        std::vector<std::vector<Route>>& paths) const;
};
