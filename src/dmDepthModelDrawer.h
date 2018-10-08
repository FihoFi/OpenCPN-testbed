#pragma once

#ifndef _DM_DEPTH_MODEL_DRAWER_
#define _DM_DEPTH_MODEL_DRAWER_

#include "wx/wxprec.h"      // contains wxDC
#include "ocpn_plugin.h"    // containsPlugIn_ViewPort
#include <wx/filename.h>    // wxFileName
#include "dmDataset.h"


class dmDepthModelDrawer
{
public:
    typedef enum chartState { UNSET, FILE_SET, PROJECTION_OK, CHART_AREA_OK, BITMAP_AVAILABLE } chartState;
    typedef enum crdSystem { UTM35N, WORLD_MERCATOR} crdSystem;

    dmDepthModelDrawer();
    ~dmDepthModelDrawer();

    bool setDepthModelDataset(wxFileName &fileName);
    bool applyChartAreaData(coord chartTopLeft, coord chartBotRight);
    bool calculateDepthModelBitmap(PlugIn_ViewPort &vp);
    bool drawDepthChart(wxDC &dc/*, PlugIn_ViewPort &vp*/);

private:
    chartState  modelState;
    wxFileName  depthModelFileName;
    coord       imageTopLeft,  imageBotRight;
    coord       chartTopLeft,  chartBotRight;
    coord       wantedTopLeft, wantedBotRight;
    coord       lastTopLeft,   lastBotRight;


    unsigned char*  rasterToDraw;
    wxBitmap        bmp;
    wxPoint         bitmapTopLeftPositioningPoint;
    double          coords[4]; // [xMin,yMin, xMax,yMax]; x grows east, y grows north

    dmDataset   dataset;

    bool gimmeLatLons(crdSystem crdSys, /*transformation tr,*/
        double x1, double y1, double x2, double y2,
        double& lat1Out, double& lon1Out, double& lat2Out, double& lon2Out);

    void readAFile();
};

#endif _DM_DEPTH_MODEL_DRAWER_