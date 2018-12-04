#pragma once

#ifndef _DM_DEPTH_PROFILER_
#define _DM_DEPTH_PROFILER_

#include <iostream>
#include <utility>  // std::pair
#include <vector>
#include "dm_API.h" // coord
#include "dm_types.h"

class  dmDataset;
class  dmDrawingState;

class dmDepthProfile
{
public:

    dmDepthProfile(dmDataset& ds, const dmRoute& route);
    ~dmDepthProfile();

private:

    dmDepthProfileData getLegProfile(dmDataset& dataset, dmLeg legInWm);
    dmDepthProfileData getRouteProfile(dmDataset& dataset, dmRoute route);

    dmDepthProfileData profileData;

    std::pair<double, double> getGridSize(dmDataset &dataset);
    std::pair<double, double> getNyquistSteps(std::pair<double, double> gridSize);

};

#endif _DM_DEPTH_PROFILER_
