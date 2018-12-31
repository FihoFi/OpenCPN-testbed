/******************************************************************************
*
* Project:  OpenCPN
* Purpose:  LIVI_Depth_model Plugin
* Author:   Sitowise & LIVI, based on DR_pi example plugin code
*
***************************************************************************
*   Copyright (C) 2018 by Sitowise & LIVI                                 *
*   $EMAIL$                                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************
*/
#include "dmDepthProfile.h"

#include <algorithm>
#include <iomanip>
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
        double a = ((double)step) / steps;
        dmDepthData depthValue(routePoint, dataset.getDepthAt(routePoint));
        legProfile.push_back(depthValue);

        routePoint.north = legInWm.start.north * (1 - a) + legInWm.end.north * a;
        routePoint.east = legInWm.start.east * (1 - a) + legInWm.end.east * a;
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
        os << std::setprecision(10) << depthData.position.east << ';' <<
            depthData.position.north << ';' <<
            depthData.depth << "\r\n";
    });

    return os;
}
