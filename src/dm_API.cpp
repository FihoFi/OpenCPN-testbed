#include "dm_API.h"

dm_API::dm_API(dmLogWriter* logWriter)
    : logWriter(logWriter)
{}

dm_API::~dm_API() {}

dmLogWriter::~dmLogWriter() {}

bool dmColourTypeIsOk(DM_colourType col)
{
    if (col >= COLOUR_UNDEFINED && col < COLOUR_MAX)
        return true;
    else
        return false;
}

std::string colouringTypeToString(DM_colourType colType)
{
    switch (colType)
    {
    case COLOUR_USER_FILE:      { return std::string("User_file");     break; }
    case COLOUR_FIVE_RANGES:    { return std::string("Five_ranges");   break; }
    case COLOUR_SLIDING:        { return std::string("Sliding");       break; }
    case COLOUR_TWO_RANGES:     { return std::string("Two_ranges");    break; }
    default:                    { return std::string("Undefined");     break; }
    }
}

bool dmVisualizationIsOk(DM_visualization viz)
{
    if (viz >= VISUALIZATION_UNDEFINED && viz < VISUALIZATION_MAX)
        return true;
    else
        return false;
}

std::string chartTypeToString(DM_visualization chartType)
{
    switch (chartType)
    {
    case NONE:          { return std::string("Plain");         break; }
    case HILLSHADE:     { return std::string("Hillshade");     break; }
    case COLOR_RELIEF:  { return std::string("Color_relief");  break; }
    default:            { return std::string("Undefined");     break; }
    }
}
