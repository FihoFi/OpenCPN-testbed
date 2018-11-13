#pragma once

#ifndef _DM_DEPTH_MODEL_DRAWER_
#define _DM_DEPTH_MODEL_DRAWER_

#include "wx/wxprec.h"      // contains wxDC
#include "ocpn_plugin.h"    // contains PlugIn_ViewPort
#include <wx/filename.h>    // wxFileName

#include "dmExtent.h"
#include "dmDataset.h"
#include "dmDrawingState.h"

class dmDepthModelDrawer : public dmLogWriter
{
public:
    typedef enum crdSystem { UTM35N, WORLD_MERCATOR} crdSystem;

    dmDepthModelDrawer();
    ~dmDepthModelDrawer();

    virtual void logFatalError(const std::string message) override;
    virtual void logError     (const std::string message) override;
    virtual void logWarning   (const std::string message) override;
    virtual void logMessage   (const std::string message) override;
    virtual void logInfo      (const std::string message) override;

    DM_visualization getChartDrawType();
    bool             setChartDrawType(DM_visualization chartType);
    DM_colourType    getColourSchema();
    bool             setColourSchema(DM_colourType colourSchema);
    bool             setColourConfigurationFile(const wxFileName &fileNamePath);
    void             setTempFileFolder(wxFileName &fileName);
    bool             setDataset(const wxFileName &fileName);
    bool             openDataset(const wxFileName &fileName);
    dmExtent         applyViewPortArea(/*const*/ PlugIn_ViewPort &vp);
    bool             drawDepthChart(/*const*/ wxDC &dc, /*const*/ PlugIn_ViewPort &vp);

private:
    dmDrawingState  drawingState;

    dmDataset       dataset;
    dmExtent        wholeImageWM;   // extent (in WM) of the whole image available in the dataset

    dmExtent        croppedImageLL;
    wxBitmap*       bmp;
    wxPoint         bmpTopLeftLL;

    bool        needNewCropping         (dmExtent viewPortLL);
    dmExtent    calculateIdealCroppingLL(dmExtent viewPortLL) const;
    bool        cropImage               (dmExtent wantedCropExtentLL,
                                         dmRasterImgData** rasterOut, dmExtent& croppedImageLL, int& w, int& h);
    wxPoint     reCalculateTopLeftLocation(/*const*/PlugIn_ViewPort &vp, dmExtent croppedImageLL);
    bool        reCalculateBitmap(/*const*/ PlugIn_ViewPort &vp, const dmRasterImgData* raster,
                                   dmExtent croppedImageLL, wxBitmap& bmp, int& w, int& h, wxPoint& bmpTopLeftLL);

    void WMtoLL(const dmExtent& WMin, dmExtent& LLout);
    void LLtoWM(const dmExtent& LLin, dmExtent& WMout);

    bool gimmeLatLons(crdSystem crdSys, /*transformation tr,*/
        double x1, double y1, double x2, double y2,
        coord& Out1, coord& Out2);

    void readAFile();
};

#endif _DM_DEPTH_MODEL_DRAWER_