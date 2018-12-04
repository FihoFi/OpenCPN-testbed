#pragma once

#ifndef _DM_TYPES_H_
#define _DM_TYPES_H_

#include <vector>

#include "dmExtent.h"

struct dmDepthData
{
    dmDepthData(coord position, double depth) :
        position(position),
        depth(depth)
    { }

    coord position;
    double depth;
};

struct dmLeg
{
    coord start;
    coord end;
};

typedef std::vector<dmDepthData> dmDepthProfileData;

typedef std::vector<dmLeg> dmRoute;

#endif _DM_TYPES_H_