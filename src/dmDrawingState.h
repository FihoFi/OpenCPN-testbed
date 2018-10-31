#pragma once

#ifndef _DM_DRAWING_STATE_H_
#define _DM_DRAWING_STATE_H_


#define DM_UNSET -1

#include <wx/filename.h>

#include "dm_API.h" // DM_visualization, DM_colourType
#include "dmExtent.h"

class dmDrawingState
{
public:

    typedef enum wantedChartState {
        CHART_FILE_UNDEFINED = 125690, // Magic number differing from 0, to avoid unintentional random int values being thought of as correct values
        CHART_FILE_CHANGED,
        CHART_TYPE_UNDEFINED,
        CHART_TYPE_CHANGED,
        COLOURING_UNDEFINED,
        COLOURING_CHANGED,
        CHART_EXTENTS_UNDEFINED,
        CHART_EXTENTS_CHANGED,
        CHART_UP_TO_DATE,

        MAX_CHARTSTATE     // Not-to-be-used-in-code guardian value of the end of the enum range
    } chartState;

    dmDrawingState();

    // Current variables are to be set at the time of drawing
    bool                SetCurrentChartFileName (wxFileName fname);
    bool                SetCurrentChartType     (DM_visualization imageType);
    bool                SetCurrentColourSchema  (DM_colourType colourSchema);
    bool                SetCurrentUserColourFileName(wxFileName fname);
    bool                SetCurrentChartExtent   (dmExtent extent);

    // Wanted variables are to be set when the UI informs about changes
    bool                SetWantedChartFileName  (wxFileName fname);
    bool                SetWantedChartType      (DM_visualization imageType);
    bool                SetWantedColourSchema   (DM_colourType colourSchema);
    bool                SetWantedUserColourFileName(wxFileName fname);
    bool                SetWantedChartExtent    (dmExtent extent);

    wantedChartState    wantedChanges();

private:
    bool colouringSchemaIsOk(DM_colourType schema);
    bool filePathAndReadIsOk(wxFileName fname);

    // Current variables are to be set at the time of drawing
    wxFileName          currentChartFileName;
    DM_visualization    currentChartType;
    DM_colourType       currentColourSchema;
    wxFileName          currentUserColourFileName;
    dmExtent            currentChartExtent;

    // Wanted variables are to be set when the UI informs about changes
    wxFileName          wantedChartFileName;
    DM_visualization    wantedChartType;
    DM_colourType       wantedColourSchema;
    wxFileName          wantedUserColourFileName;
    dmExtent            wantedChartExtent;

    wantedChartState    stateOfWantedImage;
};

#endif _DM_DRAWING_STATE_H_