#pragma once

#ifndef _DM_DEPTH_PROFILER_
#define _DM_DEPTH_PROFILER_

#include <utility>  // std::pair
#include <vector>
#include "dm_API.h" // coord

class  dmDataset;
class  dmDrawingState;

class dmDepthProfiler
{
public:
    typedef std::vector<std::pair<coord, float>> dmProfile;

    dmDepthProfiler();
    ~dmDepthProfiler();

    dmProfile getLegProfile(dmDataset& dataset, coord legBeginInWM, coord legEndInWM);
    dmProfile getRouteProfile(dmDataset& dataset, std::vector<coord> route);

private:
    typedef std::pair<coord, coord>              dmLeg;
    typedef std::pair<coord, float>              dmDepthData;
    typedef std::pair<double, double>            dmSize;

    dmSize getGridSize(dmDataset &dataset);
    dmSize getNyquistSteps(std::pair<double, double> gridSize);
};

#endif _DM_DEPTH_PROFILER_
