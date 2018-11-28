
#include "dmDepthProfiler.h"

#include <utility>  // std::pair
#include <vector>

#include "dmExtent.h"   // coord
#include "dmDataset.h"

dmDepthProfiler::dmDepthProfiler()
{
}

dmDepthProfiler::~dmDepthProfiler()
{
}

dmDepthProfiler::dmProfile
dmDepthProfiler::getLegProfile(dmDataset& dataset, coord legBeginInWM, coord legEndInWM)
{
    dmSize gridSize(getGridSize(dataset));       // N-S, E-W
    dmSize nyquist(getNyquistSteps(gridSize));   // N-S, E-W

    coord routePoint = legBeginInWM;
    int steps = std::abs(legBeginInWM.north - legEndInWM.north) / nyquist.first;

    dmProfile legProfile;

    for (int step = 0; step<steps; step++)
    {
        dmDepthData depthValue(routePoint, dataset.getDepthAt(routePoint));
        legProfile.push_back(depthValue);

        routePoint.north += nyquist.first;
        routePoint.east += nyquist.second;
    }

    dmDepthData depthValue(legEndInWM, dataset.getDepthAt(legEndInWM));
    legProfile.push_back(depthValue);

    return legProfile;
}

dmDepthProfiler::dmProfile
dmDepthProfiler::getRouteProfile(dmDataset& dataset, std::vector<coord> route)
{
    dmProfile routeProfile;
    if (route.empty() || route.size() < 2)
        return dmProfile();

    dmDepthData dummy;
    routeProfile.push_back(dummy);

    for (int legNr = 0; legNr<(route.size()-1); legNr++)
    {
        coord legBegin  = route.at(legNr);
        coord legEnd    = route.at(legNr+1);

        dmProfile legProfile = getLegProfile(dataset, legBegin, legEnd);
        routeProfile.erase(routeProfile.end());    // remove duplicate point; last point of prev. leg
        routeProfile.insert(routeProfile.end(), legProfile.begin(), legProfile.end());
    };

    return routeProfile;
}

dmDepthProfiler::dmSize
dmDepthProfiler::getGridSize(dmDataset &dataset)
{
    coord topLeft, botRight;
    int h, w;
    dataset.getDatasetExtents(topLeft, botRight);
    dataset.getDatasetPixelDimensions(w, h);

    return std::pair<double, double> (
        (topLeft.north - botRight.north) / h,   // N-S [WM-lat/modelPixel]
        (botRight.east - topLeft.east)   / w);  // E-W [WM-lon/modelPixel]
}

dmDepthProfiler::dmSize
dmDepthProfiler::getNyquistSteps(std::pair<double, double> gridSize)
{
    return std::pair<double, double>(
        gridSize.first / 2.0,  gridSize.second / 2.0);
}
