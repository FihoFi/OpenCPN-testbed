#include "dmDrawingState.h"
#include "dmExtent.h"
#include <wx/filename.h>

dmDrawingState::dmDrawingState()
    : currentChartFileName("")
    , currentChartType(VISUALIZATION_UNDEFINED)
    , currentColourSchema(COLOUR_UNDEFINED)
    , currentUserColourFileName("")
    , currentDrawingAreaLL()

    , wantedChartFileName("")
    , wantedChartType(VISUALIZATION_UNDEFINED)
    , wantedColourSchema(COLOUR_UNDEFINED)
    , wantedUserColourFileName("")
    , wantedDrawingAreaLL()

    , stateOfWantedImage(CHART_FILE_UNDEFINED)
{
}

void dmDrawingState::SetCurrentAsWanted()
{
    currentChartFileName      = wantedChartFileName;
    currentChartType          = wantedChartType;
    currentColourSchema       = wantedColourSchema;
    currentUserColourFileName = wantedUserColourFileName;
    currentDrawingAreaLL      = wantedDrawingAreaLL;
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

void dmDrawingState::SetWantedDrawingAreaLL(dmExtent extentLL)
{
    wantedDrawingAreaLL = extentLL;

    if (extentLL == dmExtent())
    {
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_EXTENTS_UNDEFINED);
    }
    else
    {
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_EXTENTS_CHANGED);
    }
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
