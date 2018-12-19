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
    dmLeg() :
        start(coord()),
        end(coord())
    {
    }

    dmLeg(coord start, coord end) :
        start(start),
        end(end)
    {
    }

    coord start;
    coord end;
};

typedef std::vector<dmDepthData> dmDepthProfileData;

typedef std::vector<dmLeg> dmRoute;

#endif _DM_TYPES_H_