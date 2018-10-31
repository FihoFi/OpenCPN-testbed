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

bool dmVisualizationIsOk(DM_visualization viz)
{
    if (viz >= VISUALIZATION_UNDEFINED && viz < VISUALIZATION_MAX)
        return true;
    else
        return false;
}