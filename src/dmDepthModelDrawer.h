#pragma once

#ifndef _DM_DEPTH_MODEL_DRAWER_
#define _DM_DEPTH_MODEL_DRAWER_

#include "wx/wxprec.h"      // contains wxDC
#include "ocpn_plugin.h"    // containsPlugIn_ViewPort
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

    bool setDepthModelDataset(const wxFileName &fileName);
    bool hasDataset();
    bool applyChartArea(coord chartTopLeft, coord chartBotRight);
    bool applyChartArea(PlugIn_ViewPort &vp);
    bool drawDepthChart(wxDC &dc, PlugIn_ViewPort &vp);
    bool reCalculateDepthModelBitmap(PlugIn_ViewPort &vp);

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

private:
    bool        chartAreaKnown;
    bool        datasetAvailable;

    wxFileName  depthModelFileName;
    dmDataset   dataset;
    coord       wholeImageTopLeftWM,  wholeImageBotRightWM;
    coord       croppedImageTopLeftWM, croppedImageBotRightWM;
    dmDrawingState  drawingState;

    coord       imageTopLeftLL,  imageBotRightLL;
    coord       chartTopLeftLL,  chartBotRightLL;
    coord       idealTopLeftLL,  idealBotRightLL;
    coord       lastTopLeftLL,   lastBotRightLL;


    dmRasterImgData*  raster;
    int             w, h; // image width, and height
    wxBitmap        bmp;
    wxPoint         bitmapTopLeftPositioningPoint;


    bool calculateWholeWMProjectedImage();
    bool calculateCroppedWMProjectedImage();
    void WMtoLL(const coord& topLeftWMin, const coord& botRightWMin,
                coord& topLeftLLout, coord& botRightLLout);
    bool needANewCropping();
    void calculateIdealImageCroppingLL();
    bool gimmeLatLons(crdSystem crdSys, /*transformation tr,*/
        double x1, double y1, double x2, double y2,
        double& lat1Out, double& lon1Out, double& lat2Out, double& lon2Out);

    void readAFile();
};

#endif _DM_DEPTH_MODEL_DRAWER_