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
