#pragma once

#ifndef _DM_DRAWING_STATE_H_
#define _DM_DRAWING_STATE_H_


#define DM_UNSET -1

#include <wx/filename.h>

#include "dm_API.h" // DM_visualization
#include "dmExtent.h"

class dmDrawingState
{
public:

    typedef enum colouringSchema {
        UNDEFINED_COLOURS = 124578, // Magic number differing from 0, to avoid unintentional random int values being thought of as correct values
        USER_FILE_COLOURS,
        FIVE_COLOURS,
        TWO_COLOURS,
        SLIDING_COLOURS,

        MAX_COLOURS     // Not-to-be-used-in-code guardian value of the end of the enum range
    } colSchema;

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
    bool                SetCurrentChartType(DM_visualization imageType);
    bool                SetCurrentColourSchema  (colouringSchema colourSchema);
    bool                SetCurrentUserColourFileName(wxFileName fname);
    bool                SetCurrentChartExtent   (dmExtent extent);

    // Wanted variables are to be set when the UI informs about changes
    bool                SetWantedChartFileName (wxFileName fname);
    bool                SetWantedChartType(DM_visualization imageType);
    bool                SetWantedColourSchema  (colouringSchema colourSchema);
    bool                SetWantedUserColourFileName(wxFileName fname);
    bool                SetWantedChartExtent   (dmExtent extent);

    wantedChartState    wantedChanges();

private:
    bool colouringSchemaIsOk(colouringSchema schema);
    bool filePathAndReadIsOk(wxFileName fname);

    // Current variables are to be set at the time of drawing
    wxFileName          currentChartFileName;
    DM_visualization    currentChartType;
    colouringSchema     currentColourSchema;
    wxFileName          currentUserColourFileName;
    dmExtent            currentChartExtent;

    // Wanted variables are to be set when the UI informs about changes
    wxFileName          wantedChartFileName;
    DM_visualization    wantedChartType;
    colouringSchema     wantedColourSchema;
    wxFileName          wantedUserColourFileName;
    dmExtent            wantedChartExtent;

    wantedChartState    stateOfWantedImage;
};

#endif _DM_DRAWING_STATE_H_