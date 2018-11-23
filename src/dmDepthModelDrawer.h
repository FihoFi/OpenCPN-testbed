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

    void             getCurrents(DM_visualization& chartType,
                                 DM_colourType& currentColourSchema,
                                 double& currentWaterLevel,
                                 double& vertRefSystOffset);
    wxFileName       getChartFileName();
    bool             setChartFileName(const wxFileName &fileNamePath);
    DM_visualization getChartDrawType();
    bool             setChartDrawType(DM_visualization chartType);
    DM_colourType    getColourSchema();
    bool             setColourSchema(DM_colourType colourSchema);
    bool             setColourConfigurationFile(const wxFileName &fileNamePath);
    void             setCurrentWaterLevel(double cvl);
    void             setVerticalReferenceSystemOffset(double vrso);
    void             setTempFileFolder(wxFileName &fileName);

    bool             setDataset(const wxFileName &fileName);
    bool             openDataset();
    dmExtent         applyViewPortArea(/*const*/ PlugIn_ViewPort &vp);

    bool             isRendering()      { return renderingDmChart; }
    void             setRenderingOn()   { renderingDmChart = true; }
    void             setRenderingOff()  { renderingDmChart = false; }
    void             forceNewImage();
    bool             drawDepthChart(/*const*/ wxDC &dc, /*const*/ PlugIn_ViewPort &vp);

    bool             isShowingDepthValue()      { return showingDepthValue;  }
    void             setShowingDepthValueOn()   { showingDepthValue = true;  }
    void             setShowingDepthValueOff()  { showingDepthValue = false; }
    void             SetCursorLatLon(double lat, double lon);
    void             SetCursorPix(wxPoint position);
    bool             drawDepthValue(wxDC &dc, PlugIn_ViewPort &vp);

    float _depth;
    double _lat, _lon;
    wxPoint _pix;

private:
    bool            showingDepthValue;
    bool            renderingDmChart;
    bool            mustGetNewBmp;
    bool            newDepthValueCalledOnly;

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

    void readAFile();

};

#endif _DM_DEPTH_MODEL_DRAWER_