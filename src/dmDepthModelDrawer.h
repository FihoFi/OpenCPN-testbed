#pragma once

#ifndef _DM_DEPTH_MODEL_DRAWER_
#define _DM_DEPTH_MODEL_DRAWER_

#include "wx/wxprec.h"      // wxDC
#include "ocpn_plugin.h"    // PlugIn_ViewPort
#include <wx/filename.h>    // wxFileName
#include "dmDataset.h"

typedef enum modelState { UNSET, FILE_SET, CHART_AREA_OK, BITMAP_AVAILABLE } modelState;

class dmDepthModelDrawer
{
public:
    dmDepthModelDrawer();
    ~dmDepthModelDrawer();

    bool setDepthModelDataset(wxFileName &fileName);
    bool applyChartAreaData(coord chartTopLeft, coord chartBotRight);
    bool calculateDepthModelBitmap(PlugIn_ViewPort &vp);
    bool drawDepthChart(wxDC &dc/*, PlugIn_ViewPort &vp*/);

private:
    modelState  modelState;
    wxFileName  depthModelFileName;
    coord       lastTopLeft,      lastBotRight;
    coord       wantedTopLeft,    wantedBotRight;


    unsigned char*  rasterToDraw;
    wxImage         original;
    wxBitmap        bmp;
    wxPoint         bitmapTopLeftPositioningPoint;
    double          coords[4]; // [xMin,yMin, xMax,yMax]; x grows east, y grows north

    dmDataset   dataset;

    bool gimmeLatLons(/*transformation tr,*/
        double x1, double y1, double x2, double y2,
        double& lat1Out, double& lon1Out, double& lat2Out, double& lon2Out);

    void readAFile();
};

#endif _DM_DEPTH_MODEL_DRAWER_