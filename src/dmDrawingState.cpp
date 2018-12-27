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
#include "dmDrawingState.h"
#include "dmExtent.h"
#include <wx/filename.h>

dmDrawingState::dmDrawingState()
    : currentChartFileName("")
    , currentChartType(VISUALIZATION_UNDEFINED)
    , currentColourSchema(COLOUR_UNDEFINED)
    , currentUserColourFileName("")
    , currentCurrentWaterLevel(0)
    , currentVertRefSystOffset(0)

    , wantedChartFileName("")
    , wantedChartType(VISUALIZATION_UNDEFINED)
    , wantedColourSchema(COLOUR_UNDEFINED)
    , wantedUserColourFileName("")
    , wantedCurrentWaterLevel(0)
    , wantedVertRefSystOffset(0)

    , stateOfWantedImage(CHART_FILE_UNDEFINED)
{
}

void dmDrawingState::SetCurrentAsWanted()
{
    currentChartFileName      = wantedChartFileName;
    currentChartType          = wantedChartType;
    currentColourSchema       = wantedColourSchema;
    currentUserColourFileName = wantedUserColourFileName;
    currentCurrentWaterLevel  = wantedCurrentWaterLevel;
    currentVertRefSystOffset  = wantedVertRefSystOffset;
}

void  dmDrawingState::GetCurrentWaterLevels(
    double& currentWaterLevel, double& vertRefSystOffset)
{
    currentWaterLevel = currentCurrentWaterLevel;
    vertRefSystOffset = currentVertRefSystOffset;
}

void  dmDrawingState::GetCurrents(DM_visualization& chartType, DM_colourType& colourSchema,
                                  double& currentWaterLevel, double& vertRefSystOffset)
{
    chartType         = currentChartType;
    colourSchema      = currentColourSchema;
    currentWaterLevel = currentCurrentWaterLevel;
    vertRefSystOffset = currentVertRefSystOffset;
}

bool dmDrawingState::SetWantedChartFileName(wxFileName fname)
{
    if (filePathAndReadIsOk(fname))
    {
        if (!fname.SameAs(wantedChartFileName))
        {
            wantedChartFileName = fname;
            stateOfWantedImage = std::min(stateOfWantedImage, CHART_FILE_CHANGED);
        }
        return true;
    }
    else
    {
        wantedChartFileName = wxFileName();
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_FILE_UNDEFINED);
        return false;
    }
}

bool dmDrawingState::SetWantedChartType(DM_visualization imageType)
{
    if (dmVisualizationIsOk(imageType))
    {
        wantedChartType = imageType;
        return true;
    }
    else
    {
        return false;
    }
}

bool dmDrawingState::SetWantedColourSchema(DM_colourType colourSchema)
{
    if (dmColourTypeIsOk(colourSchema))
    {
        wantedColourSchema = colourSchema;
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_TYPE_UNDEFINED);
        return true;
    }
    else
    {
        wantedColourSchema = COLOUR_UNDEFINED;
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_TYPE_CHANGED);
        return false;
    }
}

bool dmDrawingState::SetWantedUserColourFileName(wxFileName fname)
{
    if (filePathAndReadIsOk(fname))
    {
        if (!fname.SameAs(wantedUserColourFileName))
        {
            wantedUserColourFileName = fname;
            stateOfWantedImage = std::min(stateOfWantedImage, COLOURING_CHANGED);
        }
        return true;
    }
    else
    {
        wantedUserColourFileName = wxFileName();
        stateOfWantedImage = std::min(stateOfWantedImage, COLOURING_UNDEFINED);
        return false;
    }
}

void dmDrawingState::SetWantedCurrentWaterLevel(double cwl)
{
    wantedCurrentWaterLevel = cwl;
    stateOfWantedImage = std::min(stateOfWantedImage, COLOURING_CHANGED);
}

void dmDrawingState::SetWantedVerticalReferenceSystemOffset(double vrso)
{
    wantedVertRefSystOffset = vrso;
    stateOfWantedImage = std::min(stateOfWantedImage, COLOURING_CHANGED);
}

dmDrawingState::wantedChartState dmDrawingState::wantedChanges()
{
    if (wantedChartFileName != currentChartFileName)
    {
        return CHART_FILE_CHANGED;
    }

    if (wantedChartType != currentChartType)
    {
        return CHART_TYPE_CHANGED;
    }

    if (currentChartType == COLOR_RELIEF)
    {
        if (wantedColourSchema != currentColourSchema)
        {
            return COLOURING_CHANGED;
        }
        if (currentColourSchema == COLOUR_USER_FILE)
        {
            if (wantedUserColourFileName != wantedUserColourFileName)
            {
                return COLOURING_CHANGED;
            }
        }
    }
    return CHART_UP_TO_DATE;
}

bool dmDrawingState::filePathAndReadIsOk(wxFileName fname)
{
    wxString path = fname.GetFullPath();
    wxFile file(path, wxFile::read);
    if (file.Exists(path))
    {
        file.Close();
        return true;
    }
    else
    {
        return false;
    }
}
