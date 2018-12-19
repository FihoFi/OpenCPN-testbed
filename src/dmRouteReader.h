#pragma once

#ifndef _DM_ROUTE_READER_
#define _DM_ROUTE_READER_

#include "dm_types.h"

class TiXmlElement;

class dmRouteReader
{
public:
    dmRouteReader();

    dmRoute readRouteFromGpxFile(const char* fname);
    dmRoute readRouteFromGpxFile(std::string fname);
    bool parseRteptCoordinates(TiXmlElement *rteptElement, coord &coords);
};

#endif _DM_ROUTE_READER_