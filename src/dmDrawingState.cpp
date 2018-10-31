#include "dmDrawingState.h"
#include "dmExtent.h"
#include <wx/filename.h>

dmDrawingState::dmDrawingState()
    : currentChartFileName("")
    , currentChartType(VISUALIZATION_UNDEFINED)
    , currentColourSchema(UNDEFINED_COLOURS)
    , currentUserColourFileName("")
    , currentChartExtent()

    , wantedChartFileName("")
    , wantedChartType(VISUALIZATION_UNDEFINED)
    , wantedColourSchema(UNDEFINED_COLOURS)
    , wantedUserColourFileName("")
    , wantedChartExtent()

    , stateOfWantedImage(CHART_FILE_UNDEFINED)
{
}

bool dmDrawingState::SetCurrentChartFileName(wxFileName fname)
{
    if (!filePathAndReadIsOk(fname))
    {    return false;   }

    currentChartFileName = fname;
    return true;
}

bool dmDrawingState::SetCurrentChartType(DM_visualization imageType)
{
    if(dmVisualizationIsOk(imageType))
    {
        currentChartType = imageType;
        return true;
    }
    else
    {
        return false;
    }
}

bool dmDrawingState::SetCurrentColourSchema(colouringSchema colourSchema)
{
    if (colouringSchemaIsOk(colourSchema))
    {
        currentColourSchema = colourSchema;
        return true;
    }
    else
    {
        return false;
    }
}

bool dmDrawingState::SetCurrentUserColourFileName(wxFileName fname)
{
    if (!filePathAndReadIsOk(fname))
    {    return false;    }

    currentUserColourFileName = fname;
    return true;
}

bool dmDrawingState::SetCurrentChartExtent(dmExtent extent)
{
    if (extent == dmExtent())
    {
        throw(std::string("There should be no zero valued dmExtent sent to SetCurrentChartExtent"));
        return false;
    }
    else
    {
        currentChartExtent = extent;
        return true;
    }
}

bool dmDrawingState::SetWantedChartFileName(wxFileName fname)
{
    if (filePathAndReadIsOk(fname))
    {
        wantedChartFileName = fname;
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_FILE_CHANGED);
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

bool dmDrawingState::SetWantedColourSchema(colouringSchema colourSchema)
{
    if (colouringSchemaIsOk(colourSchema))
    {
        wantedColourSchema = colourSchema;
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_TYPE_UNDEFINED);
        return true;
    }
    else
    {
        wantedColourSchema = UNDEFINED_COLOURS;
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_TYPE_CHANGED);
        return false;
    }
}

bool dmDrawingState::SetWantedUserColourFileName(wxFileName fname)
{
    if (filePathAndReadIsOk(fname))
    {
        wantedUserColourFileName = fname;
        stateOfWantedImage = std::min(stateOfWantedImage, COLOURING_CHANGED);
        return true;
    }
    else
    {
        wantedUserColourFileName = wxFileName();
        stateOfWantedImage = std::min(stateOfWantedImage, COLOURING_UNDEFINED);
        return false;
    }

}

bool dmDrawingState::SetWantedChartExtent(dmExtent extent)
{
    if (extent == dmExtent())
    {
        wantedChartExtent = extent;
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_EXTENTS_UNDEFINED);

        throw(std::string("Undefined extents sent to SetWantedChartExtent."));
        return false;
    }
    else
    {
        wantedChartExtent = extent;
        stateOfWantedImage = std::min(stateOfWantedImage, CHART_EXTENTS_CHANGED);
        return true;
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

    if (currentChartType == COLOR_RELIEF /*DM_viz_COLOR_RELIEF*/)
    {
        if (wantedColourSchema != currentColourSchema)
        {
            return COLOURING_CHANGED;
        }
        if (currentColourSchema == USER_FILE_COLOURS /*DM_viz_USER_FILE*/)
        {
            if (wantedUserColourFileName != wantedUserColourFileName)
            {
                return COLOURING_CHANGED;
            }
        }
    }
    return CHART_UP_TO_DATE;
}

bool dmDrawingState::colouringSchemaIsOk(colouringSchema schema)
{
    if (schema >= UNDEFINED_COLOURS && schema < MAX_COLOURS)
        return true;
    else
        return false;
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
