//#include "gdal_priv.h"
//#include "cpl_conv.h" // for CPLMalloc()
#include "dmDepthModelDrawer.h"

#include <proj_api.h>
#include "dmExtent.h"
#include "dm_API.h"
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
    , dataset(this), raster(NULL), w(0), h(0)
{    }

dmDepthModelDrawer::~dmDepthModelDrawer()
{    delete raster;    }

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

DM_visualization dmDepthModelDrawer::getChartDrawType()
{
    return drawingState.GetWantedChartType();
}

bool dmDepthModelDrawer::setChartDrawType(DM_visualization chartType)
{
    bool success = drawingState.SetWantedChartType(chartType);
    success &= dataset.setVisualizationScheme(chartType);
    return success;
}

DM_colourType dmDepthModelDrawer::getColourSchema()
{
    return drawingState.GetWantedColourSchema();
}

bool dmDepthModelDrawer::setColourSchema(DM_colourType colourSchema)
{
    bool success = drawingState.SetWantedColourSchema(colourSchema);
    // TODO set to dataset?
    return success;
}

bool dmDepthModelDrawer::setColourConfigurationFile(const wxFileName &fileNamePath)
{
    bool success = drawingState.SetWantedUserColourFileName(fileNamePath);

    wxString    fileNameWxStr = fileNamePath.GetFullPath();
    std::string fileNameStr = fileNameWxStr.ToStdString();
    const char* fileNameCharPtr = fileNameStr.c_str();

    success &= dataset.setColourConfigurationFile(fileNameCharPtr, false);
    return success;
}

/**
* Asks dmDataset to open the dataset in the file <i>fileName</i>, and queries
* the (World Mercator) extents of the dataset.
*/
bool dmDepthModelDrawer::setDataset(const wxFileName &fileName)
{
    bool success = drawingState.SetWantedChartFileName(fileName);
    // TODO set to dataset?
    return success;
}

bool dmDepthModelDrawer::openDataset(const wxFileName &fileName)
{
    wxString    fileNameWxStr = fileName.GetFullPath();
    std::string fileNameStr = fileNameWxStr.ToStdString();

    if (!fileName.SameAs(drawingState.GetWantedChartFileName()))
    {
        wxLogMessage(_T("dmDepthModelDrawer::openDataset failed, the chart file "
                        "is not set: ") + fileNameStr);
        return false;
    }

    const char* fileNameCharPtr = fileNameStr.c_str();
    bool success = dataset.openDataSet(fileNameCharPtr);
    if (!success)
    {
        wxLogMessage(_T("dmDepthModelDrawer::openDataset openDataSet failed: ") + fileNameStr);
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

    dmExtent wantedDrawingArea = calculateIdealCroppingLL(vpLL);
    drawingState.SetWantedDrawingAreaLL(wantedDrawingArea);

    return vpLL;
}

bool dmDepthModelDrawer::drawDepthChart(wxDC &dc, PlugIn_ViewPort &vp)
{
    dmExtent vpExtentLL = applyViewPortArea(vp);
    bool success;
    if (needNewCropping(vpExtentLL))
    {
        dmExtent idealCroppingLL = calculateIdealCroppingLL(vpExtentLL);
        success = cropImage(idealCroppingLL, &raster, croppedImageLL, w,h);
        if(!success)
        {
            wxLogMessage(_T("dmDepthModelDrawer::drawDepthChart - Crop failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }

        success = reCalculateBitmap(vp, raster, croppedImageLL,
                                    *bmp, w, h, bmpTopLeftLL);
        success = reCalculateDepthModelBitmap(vp);

    }
    else
    {
        bmpTopLeftLL = reCalculateTopLeftLocation(vp, croppedImageLL);
    }

    //wxString  fname = "C:\\OPENCPN_DATA\\UkiImg_wm.png";
    if(success)
        dc.DrawBitmap(bmp, bmpTopLeftLL, true);

    return true;
}

wxPoint dmDepthModelDrawer::reCalculateTopLeftLocation(/*const*/PlugIn_ViewPort &vp, dmExtent croppedImageLL)
{
    wxPoint r1;
    GetCanvasPixLL(&vp, &r1, croppedImageLL.topLeft.north, croppedImageLL.topLeft.east);   // up-left
    return r1;
}

bool dmDepthModelDrawer::reCalculateDepthModelBitmap(PlugIn_ViewPort &vp)
{
    // Get min, and max coordinates where the bitmap is to be drawn
    wxPoint r1, r2;
    GetCanvasPixLL(&vp, &r1, idealTopLeftLL.north,  idealTopLeftLL.east);   // up-left
    GetCanvasPixLL(&vp, &r2, idealBotRightLL.north, idealBotRightLL.east);  // low-right

    // Calculate dimensions of the picture
    w = r2.x - r1.x; // max-min
    h = r2.y - r1.y; // max-min

    if ((w > 10 && h > 10) && (w < 10000 && h < 10000))
    {
        // Generate the image with Dataset/GDAL
        wxImage scaled;

        int newW, newH;
        dataset.getDatasetPixelDimensions(newW, newH);
        wxImage original;
        //bool loadSuccess = original.LoadFile(drawingState.GetWantedChartFileName()..GetName());
        //if (!loadSuccess)
        //{
        //    wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - LoadFile failed: ") +
        //        drawingState.GetWantedChartFileName().GetName().ToStdString());
        //    return false;
        //}
        original = wxImage(newW, newH, raster->rgb, raster->alpha, true);
        scaled = original.Scale(w, h, wxIMAGE_QUALITY_NORMAL);

        raster = NULL; // was freed by wxImage constructor

        if (!(scaled).IsOk())
        {
            wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - Scale failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }

        bmpTopLeftLL = r1;
        bmp = wxBitmap(*scaled);
        //bmp.SetMask(new wxMask(bmp, wxColour(255, 255, 255)));

        drawingState.SetCurrentAsWanted();
    }
    else
    {
        bmp = wxBitmap();
        wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - dimension fail: w,h: ") +
            wxString::Format(_T("%i"), w) + "," + wxString::Format(_T("%i"), h));
    }

    return true;
}

bool dmDepthModelDrawer::reCalculateBitmap(/*const*/PlugIn_ViewPort &vp,
    const dmRasterImgData* raster, dmExtent croppedImageLL,
    wxBitmap& bmp, int& wBmp, int& hBmp, wxPoint& bmpTopLeftLL)
{
    if (wBmp < 1 && hBmp < 1)
        bmp = wxBitmap();

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
            wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - Scale failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }


        bmp = wxBitmap(scaled);

        drawingState.SetCurrentAsWanted();
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

    dmExtent lastDrawnLL = drawingState.GetCurrentDrawingAreaLL();
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
        if (!rasterOut)
        {
            wxLogMessage(_T("dmDepthModelDrawer::cropImage - Loading the image failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }
        drawingState.SetWantedDrawingAreaLL(wantedCropExtentLL);

        success = dataset.getDatasetExtents(croppedImageWM.topLeft, croppedImageWM.botRight);
        if (!success)
        {
            wxLogMessage(_T("dmDepthModelDrawer::cropImage - Retrieving the image extents failed: ") +
                drawingState.GetWantedChartFileName().GetName().ToStdString());
            return false;
        }
        WMtoLL(croppedImageWM, croppedImageLL);

    }
    catch (const std::exception& const ex) {
        throw std::string(ex.what());
    }
    catch (const std::string& const ex) {
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

/** 
* Transforms the given two World Mercator coordinate pairs x1...y2 to 
* two WGS84 latlon coordinate pairs with PROJ(4)
* <!--openCPN transformstion functions from_XX_Plugin,-->
* according to given tr.
*/
bool dmDepthModelDrawer::gimmeLatLons(crdSystem crdSys, /*transformation tr,*/
    double x1, double y1, double x2, double y2,
    coord& Out1, coord& Out2)
{
    //if (tr < 0 || tr > 3)
    //    return false; // invalid transformation
    //
    //double x1_o = x1 - offsets[tr][0]; // lon
    //double y1_o = y1 - offsets[tr][1]; // lat
    //double x2_o = x2 - offsets[tr][0]; // 
    //double y2_o = y2 - offsets[tr][1]; //

    //if (tr == PROJ) {
        static char *EPSG32635_UTMzone35N = "+proj=utm +zone=35 +datum=WGS84 +units=m +no_defs";
        static char *EPSG3395_WorldMercator = "+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs";

        projPJ source;
        if(crdSys==UTM35N)
            source = pj_init_plus(EPSG32635_UTMzone35N);
        else //if (World Mercator)
            source = pj_init_plus(EPSG3395_WorldMercator);

        char *EPSG4326_WGS84 = "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
        projPJ target = pj_init_plus(EPSG4326_WGS84);

        if (source == NULL || target == NULL)
            return false;   // invalid transformation description
        else
        {
            double  x[2] = { x1, x2 },
                    y[2] = { y1, y2 };
            bool success = pj_transform(source, target, 2, 1, x, y, NULL);
            Out1.east  = x[0] * RAD_TO_DEG;
            Out1.north = y[0] * RAD_TO_DEG;
            Out2.east  = x[1] * RAD_TO_DEG;
            Out2.north = y[1] * RAD_TO_DEG;

            return true;
            //cout << success << endl << x << endl << y << endl;
        }
    //}
    //else if (tr == TM) {
    //    fromTM_Plugin(x1_o, y1_o, offsets[tr][2], offsets[tr][3], &lat1Out, &lon1Out);
    //    fromTM_Plugin(x2_o, y2_o, offsets[tr][2], offsets[tr][3], &lat2Out, &lon2Out);
    //}
    //else if (tr == SM) {
    //    fromSM_Plugin(x1_o, y1_o, offsets[tr][2], offsets[tr][3], &lat1Out, &lon1Out);
    //    fromSM_Plugin(x2_o, y2_o, offsets[tr][2], offsets[tr][3], &lat2Out, &lon2Out);
    //}
    //else if (tr == SM_ECC) {
    //    fromSM_ECC_Plugin(x1_o, y1_o, offsets[tr][2], offsets[tr][3], &lat1Out, &lon1Out);
    //    fromSM_ECC_Plugin(x2_o, y2_o, offsets[tr][2], offsets[tr][3], &lat2Out, &lon2Out);
    //}

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