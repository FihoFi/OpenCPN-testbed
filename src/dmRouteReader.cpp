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
#include "dmRouteReader.h"

#include "tinyxml.h"

dmRouteReader::dmRouteReader()
{
}

dmRoute dmRouteReader::readRouteFromGpxFile(const char * fname)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(fname))
    {
        return dmRoute();
    }

    dmLeg leg;
    dmRoute route;
    coord waypoint;
    coord prevWaypoint;

    TiXmlHandle docHandle(&doc);
    TiXmlElement *rteptElement = doc.FirstChild("gpx")->FirstChild("rte")->FirstChild("rtept")->ToElement();

    if (!parseRteptCoordinates(rteptElement, prevWaypoint))
    {
        return dmRoute();
    }

    rteptElement = rteptElement->NextSiblingElement();
    for (; rteptElement; rteptElement = rteptElement->NextSiblingElement())
    {
        if (!parseRteptCoordinates(rteptElement, waypoint))
        {
            route = dmRoute();
            break;
        }

        leg.start = prevWaypoint;
        leg.end = waypoint;
        route.push_back(leg);
        prevWaypoint = waypoint;
    }

    return route;
}

dmRoute dmRouteReader::readRouteFromGpxFile(std::string fname)
{
    return readRouteFromGpxFile(fname.c_str());
}

bool dmRouteReader::parseRteptCoordinates(TiXmlElement *rteptElement, coord &coords)
{
    bool success = true;
    double lat;
    double lon;
    std::string latAttribute;
    std::string lonAttribute;

    latAttribute = rteptElement->Attribute("lat");
    lonAttribute = rteptElement->Attribute("lon");
    try
    {
        lat = std::stod(latAttribute);
        lon = std::stod(lonAttribute);
        coords = coord(lat, lon);
    }
    catch (...)
    {
        //log unsuccessful conversion
        coords = coord();
        success = false;
    }

    return success;
}

