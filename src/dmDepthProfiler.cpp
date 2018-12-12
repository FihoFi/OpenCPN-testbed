
#include "dmDepthProfiler.h"

#include <algorithm>
#include <utility>  // std::pair
#include <vector>

#include "dmExtent.h"   // coord
#include "dmDataset.h"

dmDepthProfile::dmDepthProfile(dmDataset& ds, const dmRoute& route)
{
    profileData = getRouteProfile(ds, route);
}

dmDepthProfile::~dmDepthProfile()
{
}

dmDepthProfileData
dmDepthProfile::getLegProfile(dmDataset& dataset, dmLeg legInWm)
{
    std::pair<double, double> gridSize(getGridSize(dataset));       // N-S, E-W
    std::pair<double, double> nyquist(getNyquistSteps(gridSize));   // N-S, E-W

    coord routePoint = legInWm.start;
    int steps = std::abs(legInWm.start.north - legInWm.end.north) / nyquist.first;

    dmDepthProfileData legProfile;

    for (int step = 0; step<steps; step++)
    {
        dmDepthData depthValue(routePoint, dataset.getDepthAt(routePoint));
        legProfile.push_back(depthValue);

        routePoint.north += nyquist.first;
        routePoint.east += nyquist.second;
    }

    dmDepthData depthValue(legInWm.end, dataset.getDepthAt(legInWm.end));
    legProfile.push_back(depthValue);

    return legProfile;
}

dmDepthProfileData
dmDepthProfile::getRouteProfile(dmDataset& dataset, dmRoute route)
{
    dmDepthProfileData routeProfile;
    if (route.empty())
        return dmDepthProfileData();
    
    std::for_each(route.begin(), route.end(),
        [&](auto leg)
        {
            dmDepthProfileData legProfile = getLegProfile(dataset, leg);
            routeProfile.insert(routeProfile.end(), legProfile.begin(), legProfile.end()-1);
        });

    auto lastLeg = route.end() - 1;
    dmDepthData lastDepthData(lastLeg->end, dataset.getDepthAt(lastLeg->end));
    routeProfile.push_back(lastDepthData);

    return routeProfile;
}

std::pair<double, double>
dmDepthProfile::getGridSize(dmDataset &dataset)
{
    coord topLeft, botRight;
    int h, w;
    dataset.getDatasetExtents(topLeft, botRight);
    dataset.getDatasetPixelDimensions(w, h);

    return std::pair<double, double> (
        (topLeft.north - botRight.north) / h,   // N-S [WM-lat/modelPixel]
        (botRight.east - topLeft.east)   / w);  // E-W [WM-lon/modelPixel]
}

std::pair<double, double>
dmDepthProfile::getNyquistSteps(std::pair<double, double> gridSize)
{
    return std::pair<double, double>(
        gridSize.first / 2.0,  gridSize.second / 2.0);
}

std::ostream& operator<<(std::ostream& os, const dmDepthProfile& dp)
{
    std::for_each(dp.profileData.begin(), dp.profileData.end(),
        [&](dmDepthData depthData)
    {
        os << depthData.position.east << ';' <<
            depthData.position.north << ';' <<
            depthData.depth << '\n';
    });
    return os;
}
