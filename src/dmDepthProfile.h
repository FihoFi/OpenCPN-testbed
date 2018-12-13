#pragma once

#ifndef _DM_DEPTH_PROFILE_
#define _DM_DEPTH_PROFILE_

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

    friend std::ostream& operator<<(std::ostream& os, const dmDepthProfile& dp);

};

std::ostream& operator<<(std::ostream& os, const dmDepthProfile& dp);

#endif _DM_DEPTH_PROFILE_
