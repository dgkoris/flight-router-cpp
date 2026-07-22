#include "PathFinder.h"

#include <algorithm>

PathFinder::PathFinder(const std::vector<Route>& routes)
{
    for (const Route& route : routes)
    {
        routesByOrigin[route.from].push_back(route);
    }
}

std::vector<std::vector<Route>> PathFinder::findAllPaths(
    const std::string& origin,
    const std::string& destination) const
{
    std::vector<std::vector<Route>> paths;
    std::unordered_set<std::string> visited;
    std::vector<Route> currentPath;

    findPaths(
        origin,
        destination,
        visited,
        currentPath,
        paths);

    std::stable_sort(
        paths.begin(),
        paths.end(),
        [](const std::vector<Route>& first,
            const std::vector<Route>& second)
    {
        return first.size() < second.size();
    });

    return paths;
}

void PathFinder::findPaths(
    const std::string& current,
    const std::string& destination,
    std::unordered_set<std::string>& visited,
    std::vector<Route>& currentPath,
    std::vector<std::vector<Route>>& paths) const
{
    if (current == destination)
    {
        paths.push_back(currentPath);
        return;
    }

    visited.insert(current);

    const auto routesIterator = routesByOrigin.find(current);

    if (routesIterator != routesByOrigin.end())
    {
        for (const Route& route : routesIterator->second)
        {
            if (visited.find(route.to) != visited.end())
            {
                continue;
            }

            currentPath.push_back(route);

            findPaths(
                route.to,
                destination,
                visited,
                currentPath,
                paths);

            currentPath.pop_back();
        }
    }

    visited.erase(current);
}
