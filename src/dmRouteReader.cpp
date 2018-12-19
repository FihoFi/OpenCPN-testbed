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

