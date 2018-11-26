//#include "gdal_priv.h"
//#include "cpl_conv.h" // for CPLMalloc()
#include <iomanip> // setprecision
#include <sstream> // stringstream

#include "dmDepthModelDrawer.h"

#include <proj_api.h>
#include "dmExtent.h"
#include "dmDataset.h"
#include "dmDrawingState.h"


#include <iostream>
#include <sstream>
// This allows you to say things like : 
// DBOUT("The value of x is " << x);
// to output window.
#define DBOUT( s ) \
{ std::wostringstream os_;  os_ << s; OutputDebugString( os_.str().c_str() ); }


dmDepthModelDrawer::dmDepthModelDrawer()
    : drawingState()
    , dataset(this)
    , renderingDmChart(false)
    , showingDepthValue(true)
    , _depth(0.0), _lat(0.0), _lon(0.0), _pix()
{
    bmp = NULL;
}

dmDepthModelDrawer::~dmDepthModelDrawer()
{
    if (bmp) { delete bmp; bmp = NULL; }
}

void dmDepthModelDrawer::logFatalError(const std::string message)
{    wxLogFatalError(wxString(message));    DBOUT("FatalError: "+message+"\n");  }
void dmDepthModelDrawer::logError(const std::string message)
{    wxLogError(wxString(message));         DBOUT("Error: " + message + "\n"); }
void dmDepthModelDrawer::logWarning(const std::string message)
{    wxLogWarning(wxString(message));       DBOUT("Warning: " + message + "\n");}
void dmDepthModelDrawer::logMessage(const std::string message)
{    wxLogMessage(wxString(message));       DBOUT("Message: " + message + "\n");}
void dmDepthModelDrawer::logInfo(const std::string message)
{    wxLogInfo(wxString(message));          DBOUT("Info: " + message + "\n");  }

void dmDepthModelDrawer::getCurrents(DM_visualization& chartType,
    DM_colourType& currentColourSchema,
    double& currentWaterLevel,
    double& vertRefSystOffset)
{
    drawingState.GetCurrents(chartType, currentColourSchema,
        currentWaterLevel, vertRefSystOffset);
}

wxFileName dmDepthModelDrawer::getChartFileName()
{
    return drawingState.GetWantedChartFileName();
}

bool dmDepthModelDrawer::setChartFileName(const wxFileName &fileNamePath)
{
    return drawingState.SetWantedChartFileName(fileNamePath);
}

DM_visualization dmDepthModelDrawer::getChartDrawType()
{
    return drawingState.GetWantedChartType();
}

bool dmDepthModelDrawer::setChartDrawType(DM_visualization chartType)
{
    return drawingState.SetWantedChartType(chartType);
}

DM_colourType dmDepthModelDrawer::getColourSchema()
{
    return drawingState.GetWantedColourSchema();
}

bool dmDepthModelDrawer::setColourSchema(DM_colourType colourSchema)
{
    bool success = drawingState.SetWantedColourSchema(colourSchema);
    return success;
}

bool dmDepthModelDrawer::setColourConfigurationFile(const wxFileName &fileNamePath)
{
    return drawingState.SetWantedUserColourFileName(fileNamePath);
}

void dmDepthModelDrawer::setCurrentWaterLevel(double cvl)
{
    drawingState.SetWantedCurrentWaterLevel(cvl);
}

void dmDepthModelDrawer::setVerticalReferenceSystemOffset(double vrso)
{
    drawingState.SetWantedVerticalReferenceSystemOffset(vrso);
}

void dmDepthModelDrawer::setTempFileFolder(wxFileName &fileName)
{
    dataset.setTempFolderPath(fileName.GetPath().ToStdString());
}

/**
* Asks dmDataset to open the dataset in the file <i>fileName</i>, and queries
* the (World Mercator) extents of the dataset.
*/
bool dmDepthModelDrawer::setDataset(const wxFileName &fileName)
{
    // Check existance of the file
    wxString path = fileName.GetFullPath();

    wxFile file(path, wxFile::read); // this opens the file, if it exists, remember to close!
    if (!file.Exists(path))
    {
        return false;
    }
    else if (file.IsOpened())
    {
        file.Close();
    }

    return drawingState.SetWantedChartFileName(fileName);
}

bool dmDepthModelDrawer::openDataset()
{
    wxFileName  colourFileNamePath    = drawingState.GetWantedUserColourFileName();
    wxString    colourFileNameWxStr   = colourFileNamePath.GetFullPath();
    std::string colourFileNameStr     = colourFileNameWxStr.ToStdString();
    const char* colourFileNameCharPtr = colourFileNameStr.c_str();

    bool success = dataset.setColourConfigurationFile(colourFileNameCharPtr, false);
    if (!success)
    {
        wxLogError(_T("dmDepthModelDrawer::openDataset setColourConfigurationFile failed: ") + colourFileNameWxStr);
        return false;
    }

    DM_visualization chartType = drawingState.GetWantedChartType();
    success = dataset.setVisualizationScheme(chartType);
    if (!success)
    {
        wxLogError(_T("dmDepthModelDrawer::openDataset setVisualizationScheme failed: ") + chartType);
        return false;
    }

    wxFileName  fileName = drawingState.GetWantedChartFileName();
    std::string fileNameStr = fileName.GetFullPath().ToStdString();
    const char* fileNameCharPtr = fileNameStr.c_str();
    success &= dataset.openDataSet(fileNameCharPtr);
    if (!success)
    {
        wxLogError(_T("dmDepthModelDrawer::openDataset openDataSet failed: ") + fileNameStr);
        return false;
    }

    success = dataset.getDatasetExtents(wholeImageWM.topLeft, wholeImageWM.botRight);

    return success;
}

/**
* Applies the extent of the vp area we want to draw to,
* causing an update to the wanted drawn area.
*
* @return The dmExtent of <i>vp</i> in LatLons.
*/
dmExtent dmDepthModelDrawer::applyViewPortArea(PlugIn_ViewPort &vp)
{
    coord topLeftLL(vp.lat_max, vp.lon_min);
    coord botRightLL(vp.lat_min, vp.lon_max);
    dmExtent vpLL(topLeftLL, botRightLL);

    return vpLL;
}

void dmDepthModelDrawer::forceNewImage()
{
    mustGetNewBmp = true;
}

bool dmDepthModelDrawer::drawDepthValue(wxDC &dc, PlugIn_ViewPort &vp)
{
    if (!showingDepthValue || bmp == NULL)
        return true;

    if (abs(_lat) < 0.00001 && abs(_lon) < 0.00001) {
        // lat/lon are zeroes, transform pixel values to lat/lon
        GetCanvasLLPix(&vp, _pix, &_lat, &_lon);
    }
    else if (_pix == wxPoint()) {
        // pix has default value, transform lat/lon value to pixel values
        GetCanvasPixLL(&vp, &_pix, _lat, _lon);
    }
    else {
        // Neither wxPoint, nor lat/lon has default value, this is
        // a corner case, where the values are scrambled. 
        return false;
    }
    dmExtent extWM;
    LLtoWM(dmExtent(coord(_lat, _lon), coord()), extWM);

    double modelDepth = dataset.getDepthAt(extWM.topLeft);
    if (modelDepth < -9998)
        return true;

    double currentWL, vertRefSyst;
    drawingState.GetCurrentWaterLevels(currentWL, vertRefSyst);

    // keep systemCorrected depth unchanged, if it has "out-of-model" value
    float systemCorrectedDepth = modelDepth + (modelDepth < -9999 ? (float)vertRefSyst : 0.0);
    float wholeDepth = systemCorrectedDepth + (float)currentWL;
    std::string sign = currentWL > 0.0 ? " +" : " ";

    std::stringstream stream;
    stream  << std::fixed << std::setprecision(2)
            << wholeDepth << " (" << systemCorrectedDepth << sign << currentWL << ")";

    std::string depthStr = stream.str();
    dc.DrawText(wxString(depthStr), _pix.x, _pix.y-10);

    newDepthValueCalledOnly = false;
    return true;
}

bool dmDepthModelDrawer::drawDepthChart(wxDC &dc, PlugIn_ViewPort &vp)
{
    if (!renderingDmChart)
        return true;

    dmExtent vpExtentLL = applyViewPortArea(vp);
    bool success;
    dmRasterImgData* raster;
    int             w, h;
    raster = NULL;


    if (!newDepthValueCalledOnly || bmp == NULL || 
        needNewCropping(vpExtentLL) || mustGetNewBmp)
    {
        dmExtent idealCroppingLL = calculateIdealCroppingLL(vpExtentLL);
        success = cropImage(idealCroppingLL, &raster, croppedImageLL, w,h);
        if(!success)
        {
            wxLogMessage(_T("dmDepthModelDrawer::drawDepthChart - Crop failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }
        if (w < 1 || h < 1)
        {
            return true;    // the picture of Nothing drawn.
        }

        if (bmp == NULL) { bmp = new wxBitmap(); }
        success = reCalculateBitmap(vp, raster, croppedImageLL,
                                    *bmp, w, h, bmpTopLeftLL);
        if (!success)
        {
            wxLogMessage(_T("dmDepthModelDrawer::drawDepthChart - Bitmap reCalculation failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            if (bmp)
            {
                delete bmp;
                bmp = NULL;
            }
            
            return false;
        }
        else
        {
            drawingState.SetCurrentAsWanted();
            mustGetNewBmp = false;  // new bmp retrieved; clear the flag
        }
    }
    else
    {
        bmpTopLeftLL = reCalculateTopLeftLocation(vp, croppedImageLL);
    }

    if (bmp)
    {
        dc.DrawBitmap(*bmp, bmpTopLeftLL, true);
    }

    return true;
}

void dmDepthModelDrawer::SetCursorLatLon(double lat, double lon)
{
    _lat = lat;
    _lon = lon;
    _pix = wxPoint();
    newDepthValueCalledOnly = true;
}

void dmDepthModelDrawer::SetCursorPix(wxPoint position)
{
    _lat = 0;
    _lon = 0;
    _pix = position;
    newDepthValueCalledOnly = true;
}

wxPoint dmDepthModelDrawer::reCalculateTopLeftLocation(/*const*/PlugIn_ViewPort &vp, dmExtent croppedImageLL)
{
    wxPoint r1;
    GetCanvasPixLL(&vp, &r1, croppedImageLL.topLeft.north, croppedImageLL.topLeft.east);   // up-left
    return r1;
}

bool dmDepthModelDrawer::reCalculateBitmap(/*const*/PlugIn_ViewPort &vp,
    const dmRasterImgData* raster, dmExtent croppedImageLL,
    wxBitmap& bmp, int& wBmp, int& hBmp, wxPoint& bmpTopLeftLL)
{
    if (wBmp < 1 || hBmp < 1)
    {
        bmp = wxBitmap();
        return true;    // here is nothing to calculate
    }
    // Get min, and max coordinates where the bitmap is to be drawn
    wxPoint r1, r2;
    GetCanvasPixLL(&vp, &r1, croppedImageLL.topLeft.north,  croppedImageLL.topLeft.east);   // up-left
    GetCanvasPixLL(&vp, &r2, croppedImageLL.botRight.north, croppedImageLL.botRight.east);  // low-right

    // Calculate dimensions of the picture
   int w = r2.x - r1.x; // max-min
   int h = r2.y - r1.y; // max-min
   bmpTopLeftLL = r1;

    //if ((w > 10 && h > 10) && (w < 10000 && h < 10000))
    {
        wxImage original;
        wxImage scaled;

        //bool loadSuccess = original.LoadFile(drawingState.GetWantedChartFileName()..GetName());
        //if (!loadSuccess)
        //{
        //    wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - LoadFile failed: ") +
        //        drawingState.GetWantedChartFileName().GetName().ToStdString());
        //    return false;
        //}
        original = wxImage(wBmp, hBmp, raster->rgb, raster->alpha, true);
        scaled = original.Scale(w, h, wxIMAGE_QUALITY_NORMAL);

        if (!(scaled).IsOk())
        {
            wxLogMessage(_T("dmDepthModelDrawer::reCalculateBitmap - Scale failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }

        bmp = wxBitmap(scaled);
    }
    //else
    //{
    //    bmp = wxBitmap();
    //    wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - dimension fail: w,h: ") +
    //        wxString::Format(_T("%i"), w) + "," + wxString::Format(_T("%i"), h));
    //}

    return true;
}

/**
* Compares chartXxxYyyLL, and lastXxxYyyLL coordinates, to see if the
* image extent should be calculated again, or if there is no need to.
*
* @return true, if the area spanned by lastXxxYyyLL falls inside of the
*         area spanned by chartXxxYyyLL in any direction, false else.
*/
bool dmDepthModelDrawer::needNewCropping(dmExtent viewPortLL)
{
    if (viewPortLL==dmExtent())
    {
        wxLogMessage(_T("dmDepthModelDrawer::needNewCropping does not know the canvas extent: "));
        throw (std::string("dmDepthModelDrawer::needNewCropping does not know the canvas extent"));
    }

    dmExtent lastDrawnLL = croppedImageLL;
    bool stillFits = viewPortLL.isWithin(lastDrawnLL);

    bool lastDrawnIsTooWide = false;
    lastDrawnIsTooWide |= lastDrawnLL.height() / viewPortLL.height() > 2.0;
    lastDrawnIsTooWide |= lastDrawnLL.width()  / viewPortLL.width()  > 2.0;

    if (stillFits && !lastDrawnIsTooWide)
    {   return false;   }
    else
    {   return true;    }
}

/**
* @return Ideal cropping calculated from the current viewport extents.
*/
dmExtent dmDepthModelDrawer::calculateIdealCroppingLL(dmExtent viewPortLL)  const
{
    if (viewPortLL == dmExtent())
    {
        wxLogMessage(_T("dmDepthModelDrawer::calculateIdealCroppingLL does not know the canvas extent: "));
        throw (std::string("dmDepthModelDrawer::calculateIdealCroppingLL does not know the canvas extent"));
    }

    // The "excess" part of image is now set to be an 8th part of the viewport extent.
    // Probably a better would be setting it to a sensible length value.
    double eightOfViewPortNorthLL = (viewPortLL.topLeft.north - viewPortLL.botRight.north) / 8;
    double eightOfViewPortEastLL = (viewPortLL.botRight.east - viewPortLL.topLeft.east) / 8;

    coord topLeftLL(viewPortLL.topLeft.north + eightOfViewPortNorthLL,
        viewPortLL.topLeft.east - eightOfViewPortEastLL);
    coord botRightLL(viewPortLL.botRight.north - eightOfViewPortNorthLL,
        viewPortLL.botRight.east + eightOfViewPortEastLL);
    dmExtent viewPortWithPaddingLL(topLeftLL, botRightLL);

    return viewPortWithPaddingLL;
}

bool dmDepthModelDrawer::cropImage(dmExtent wantedCropExtentLL, 
    dmRasterImgData** rasterOut, dmExtent& croppedImageLL, int& w, int& h)
{
    bool success = true;
    dmExtent tempWM;
    LLtoWM(wantedCropExtentLL, tempWM);

    try
    {
        dmExtent croppedImageWM; // extent (in WM) of the cropped image returned by getRasterData last time called
        *rasterOut = dataset.getRasterData(tempWM.topLeft, tempWM.botRight,
            croppedImageWM.topLeft, croppedImageWM.botRight, w, h);
        if (!*rasterOut)
        {
            wxLogMessage(_T("dmDepthModelDrawer::cropImage - Loading the image failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }

        WMtoLL(croppedImageWM, croppedImageLL);

    }
    catch (const std::exception& ex) {
        throw std::string(ex.what());
    }
    catch (const std::string& ex) {
        throw ex;
    }
    catch (...)
    {
        std::exception_ptr currExc = std::current_exception();
        try {
            if (currExc) {
                std::rethrow_exception(currExc);
            }
        }
        catch (const std::exception& e) {
            throw e.what();
        }
    }

    return success;
}

/**
* Returns LatLon extent corresponding to given WMin World Mercator extent.
*/
void dmDepthModelDrawer::WMtoLL(const dmExtent& WMin, dmExtent& LLout)
{
    dataset.dstSrsToLatLon(WMin, LLout);
}

/**
* Returns World Mercator extent corresponding to given LLin LatLon  extent.
*/
void dmDepthModelDrawer::LLtoWM(const dmExtent& LLin, dmExtent& WMout)
{
    dataset.latLonToDstSrs(LLin, WMout);
}

void dmDepthModelDrawer::readAFile()
{
    /*   const char*     pszFilename = "";
    GDALDataset*    poDataset;
    poDataset = (GDALDataset *)GDALOpen(pszFilename, GA_ReadOnly);

    GDALRasterBand  *poBand;
    poBand = poDataset->GetRasterBand(1);

    float *pafScanline;
    int   nXSize = poBand->GetXSize();
    pafScanline = (float *)CPLMalloc(sizeof(float)*nXSize);
    poBand->RasterIO(GF_Read, 0, 0, nXSize, 1,
    pafScanline, nXSize, 1, GDT_Float32,
    0, 0);
    */
}