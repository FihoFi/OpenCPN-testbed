//#include "gdal_priv.h"
//#include "cpl_conv.h" // for CPLMalloc()
#include "dmDepthModelDrawer.h"

#include <proj_api.h>
#include "dm_API.h"
#include "dmDataset.h"

dmDepthModelDrawer::dmDepthModelDrawer() :
    modelState(UNSET), 
    chartAreaKnown(false), datasetAvailable(false), depthModelFileName(),
    rasterToDraw(NULL), w(0), h(0)
{    }

dmDepthModelDrawer::~dmDepthModelDrawer()
{    delete rasterToDraw;    }

bool dmDepthModelDrawer::setChartDrawTypeRelief(wxFileName fileNamePath)
{
    const char* fileName = fileNamePath.GetFullPath();
    bool success = dataset.setColourConfigurationFile(fileName, false);
    success &= dataset.setVisualizationScheme(DM_visualization::COLOR_RELIEF);
    return success;
}

bool dmDepthModelDrawer::setChartDrawTypeHillshade()
{
    return dataset.setVisualizationScheme(DM_visualization::HILLSHADE);
}

bool dmDepthModelDrawer::setChartDrawTypePlain()
{
    return dataset.setVisualizationScheme(DM_visualization::NONE);
}

/**
* Asks dmDataset to open the dataset in the file <i>fileName</i>, and queries
* the World Mercator extents of the dataset.
*/
bool dmDepthModelDrawer::setDepthModelDataset(wxFileName &fileName)
{
    wxString    fileNameWxStr   = fileName.GetFullPath();
    std::string fileNameStr     = fileNameWxStr.ToStdString();
    const char* fileNameCharPtr = fileNameStr.c_str();

    if (rasterToDraw)
    {
        free(rasterToDraw);
        rasterToDraw = NULL;
    }

    bool success = dataset.openDataSet(fileNameCharPtr);
    if (success)
    {
        depthModelFileName = fileNameCharPtr;
        datasetAvailable = true;
        modelState = FILE_SET;
    }
    else
    {
        wxLogMessage(_T("dmDepthModelDrawer::setDepthModelDataset openDataSet failed: ")+ fileNameStr);
    }
    return success;
}

bool dmDepthModelDrawer::hasDataset()
{
    return datasetAvailable;
}

/**
* Saves extent of the current canvas area we are drawing to.
*/
bool dmDepthModelDrawer::applyChartArea(coord chartTopLeftLL, coord chartBotRightLL)
{
    if (modelState < FILE_SET/*PROJECTION_OK*/) { return false; }

    this->chartTopLeftLL  = chartTopLeftLL;
    this->chartBotRightLL = chartBotRightLL;
    this->chartAreaKnown = true;

    return true;
}

bool dmDepthModelDrawer::applyChartArea(PlugIn_ViewPort &vp)
{
    coord topLeftLL(vp.lat_max, vp.lon_min);
    coord botRightLL(vp.lat_min, vp.lon_max);
    return applyChartArea(topLeftLL, botRightLL);
}

bool dmDepthModelDrawer::drawDepthChart(wxDC &dc, PlugIn_ViewPort &vp)
{
    //if (modelState < CHART_AREA_OK/*BITMAP_AVAILABLE*/) { return false; }

    bool success = reCalculateDepthModelBitmap(vp);

    //wxString  fname = "C:\\OPENCPN_DATA\\UkiImg_wm.png";
    if(success)
        dc.DrawBitmap(bmp, bitmapTopLeftPositioningPoint, true);

    return true;
}

bool dmDepthModelDrawer::reCalculateDepthModelBitmap(PlugIn_ViewPort &vp)
{
    if (modelState < FILE_SET) { return false; }

    bool isNewLoad = false;

    applyChartArea(vp);
    if (needANewCropping())
    {
        try
        {
            if (rasterToDraw)
            {
                isNewLoad = false;
                calculateCroppedWMProjectedImage();
                calculateIdealImageCroppingLL();
            }
            else
            {
                isNewLoad = true;
                calculateWholeWMProjectedImage();
            }
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

        idealTopLeftLL = imageTopLeftLL;
        idealBotRightLL = imageBotRightLL;
//        lastTopLeftLL = idealTopLeftLL;
 //       lastBotRightLL = idealBotRightLL;
        modelState = PROJECTION_OK;
    }

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
        wxImage* originalFromGDAL;
        if (isNewLoad)
        {
            int newW, newH;
            dataset.getDatasetPixelDimensions(newW, newH);
            originalFromGDAL = new wxImage();
            wxImage original;
            //bool loadSuccess = original.LoadFile(depthModelFileName.GetName());
            //if (!loadSuccess)
            //{
            //    wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - LoadFile failed: ") +
            //        depthModelFileName.GetName().ToStdString());
            //    return false;
            //}
            original = wxImage(newW, newH, rasterToDraw, false);
            *originalFromGDAL = original.Scale(w, h, /*wxImageResizeQuality*/ wxIMAGE_QUALITY_NORMAL);

        }
        else
        {
            originalFromGDAL = new wxImage(w, h, rasterToDraw, false);
        }
        rasterToDraw = NULL; // was freed by wxImage constructor

        if (!(*originalFromGDAL).IsOk())
        {
            wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - Scale failed: ") +
                depthModelFileName.GetName().ToStdString());
            return false;
        }


        bitmapTopLeftPositioningPoint = r1;
        bmp = wxBitmap(*originalFromGDAL);
        //bmp.SetMask(new wxMask(bmp, wxColour(255, 255, 255)));
        delete originalFromGDAL;

        lastTopLeftLL = idealTopLeftLL;
        lastBotRightLL = idealBotRightLL;
    }
    else
    {
        bmp = wxBitmap();
        wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - dimension fail: w,h: ") +
            wxString::Format(_T("%i"), w) + "," + wxString::Format(_T("%i"), h));
    }

    modelState = BITMAP_AVAILABLE;
    return true;
}

bool dmDepthModelDrawer::calculateWholeWMProjectedImage()
{
    bool success = true;

    rasterToDraw = dataset.getRasterData(
        wholeImageTopLeftWM, wholeImageBotRightWM);
    success &= (rasterToDraw != NULL);

    if (success)
    {    modelState = CHART_AREA_OK;    }
    else
    {    return success;                }

    success &= dataset.getDatasetExtents(wholeImageTopLeftWM, wholeImageBotRightWM);
    if (success)
    {
        WMtoLL(wholeImageTopLeftWM, wholeImageBotRightWM, imageTopLeftLL, imageBotRightLL);
    }
    else
    {
        wxLogMessage(_T("dmDepthModelDrawer::calculateWholeWMProjectedImage - LoadFile failed: ") +
            depthModelFileName.GetName().ToStdString());
    }
}

bool dmDepthModelDrawer::calculateCroppedWMProjectedImage()
{
    bool success = true;

    rasterToDraw = dataset.getRasterData(
        w, h, idealTopLeftLL, idealBotRightLL,
        croppedImageTopLeftWM, croppedImageBotRightWM);

    if (success)
    {    modelState = CHART_AREA_OK;    }
    else
    {    return success;                }

    success &= dataset.getDatasetExtents(croppedImageTopLeftWM, croppedImageBotRightWM);
    if (success)
    {
        WMtoLL(croppedImageTopLeftWM, croppedImageBotRightWM, imageTopLeftLL, imageBotRightLL);
    }
    else
    {
        wxLogMessage(_T("dmDepthModelDrawer::calculateCroppedWMProjectedImage - LoadFile failed: ") +
            depthModelFileName.GetName().ToStdString());
    }
}

/**
* Compares chartXxxYyyLL, and lastXxxYyyLL coordinates, to see if the
* image extent should be calculated again, or if there is no need to.
*
* @return true, if the area spanned by lastXxxYyyLL falls inside of the
*         area spanned by chartXxxYyyLL in any direction, false else.
*/
bool dmDepthModelDrawer::needANewCropping()
{
    if (!chartAreaKnown)
    {
        wxLogMessage(_T("dmDepthModelDrawer::needANewCropping does not need the canvas extent: "));
        throw (std::string("dmDepthModelDrawer::needANewCropping does not need the canvas extent"));
    }

    if (rasterToDraw==NULL)
    {
        return true;
    }

    bool needNew = true;

    needNew |= (chartTopLeftLL.north  >= lastTopLeftLL.north);
    needNew |= (chartTopLeftLL.east   <= lastTopLeftLL.east);
    needNew |= (chartBotRightLL.north <= lastBotRightLL.north);
    needNew |= (chartBotRightLL.east  >= lastBotRightLL.east);

    return needNew;
}

/**
* Sets idealXxxYyyLL coordinates to the minimums of canvas with some padding,
* and image coordinates. In case the canvas, and image areas do not overlap,
* the coordinate pairs are set to be the same.
*
* @todo TODO Think, would the (0,0) be better than (top,left) coordinates, in case
*       of a null common area?
*/
void dmDepthModelDrawer::calculateIdealImageCroppingLL()
{
    if (!chartAreaKnown)
    {
        wxLogMessage(_T("dmDepthModelDrawer::calculateIdealImageCroppingLL does not need the canvas extent: "));
        throw (std::string("dmDepthModelDrawer::calculateIdealImageCroppingLL does not need the canvas extent"));
    }

    // TODO The "excess" part of image is now set to be an 8th part of canvas extent.
    //      The better would be a sensible length value.
    double eightOfCanvasNorthLL  = (chartTopLeftLL.north - chartBotRightLL.north) /8;
    double eightOfCanvasEastLL   = (chartBotRightLL.east - chartTopLeftLL.east)   /8;

    coord chartAndBordersTopLeftLL( chartTopLeftLL.north + eightOfCanvasNorthLL,
                                    chartTopLeftLL.east  + eightOfCanvasEastLL);
    coord chartAndBordersBotRightLL(chartBotRightLL.north - eightOfCanvasNorthLL,
                                    chartBotRightLL.east  - eightOfCanvasEastLL);

    // Select the coordinates that minimize the size of the image to be asked,
    // also taking care that we do not ask from dataset anything outside the image coordinates.
    idealTopLeftLL.north  = (imageTopLeftLL.north  < chartAndBordersTopLeftLL.north ) ? imageTopLeftLL.north  : chartAndBordersTopLeftLL.north;
    idealTopLeftLL.east   = (imageTopLeftLL.east   > chartAndBordersTopLeftLL.east  ) ? imageTopLeftLL.east   : chartAndBordersTopLeftLL.east;
    idealBotRightLL.north = (imageBotRightLL.north > chartAndBordersBotRightLL.north) ? imageBotRightLL.north : chartAndBordersBotRightLL.north;
    idealBotRightLL.east  = (imageBotRightLL.east  < chartAndBordersBotRightLL.east ) ? imageBotRightLL.east  : chartAndBordersBotRightLL.east;

    // If the canvas, and image do not have common points, shrink the area to 0 size, 
    // instead of having coordinates the wrong way around, "negative area".
    if (idealTopLeftLL.north < idealBotRightLL.north)
        idealBotRightLL.north = idealTopLeftLL.north;
    if (idealTopLeftLL.east > idealBotRightLL.east)
        idealBotRightLL.east = idealTopLeftLL.east;
}


/**
* Returns LatLon coordinates corresponding to given topLeftWMin, and botRightWMin
* World Mercator coordinates.
*/
void dmDepthModelDrawer::WMtoLL(const coord& topLeftWMin, const coord& botRightWMin,
                                coord& topLeftLLout, coord& botRightLLout)
{
    double crds[4];
    crds[0] = topLeftWMin.east;   // lon (World mercator)
    crds[1] = botRightWMin.north; // lat
    crds[2] = botRightWMin.east;  // lon
    crds[3] = topLeftWMin.north;  // lat
    gimmeLatLons(WORLD_MERCATOR, crds[0], crds[3], crds[2], crds[1],
        topLeftLLout.north, topLeftLLout.east, botRightLLout.north, botRightLLout.east);
}

/** 
* Transforms the given two World Mercator coordinate pairs x1...y2 to 
* two WGS84 latlon coordinate pairs with PROJ(4)
* <!--openCPN transformstion functions from_XX_Plugin,-->
* according to given tr.
*/
bool dmDepthModelDrawer::gimmeLatLons(crdSystem crdSys, /*transformation tr,*/
    double x1, double y1, double x2, double y2,
    double& lat1Out, double& lon1Out, double& lat2Out, double& lon2Out)
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
            lon1Out = x[0] * RAD_TO_DEG;
            lat1Out = y[0] * RAD_TO_DEG;
            lon2Out = x[1] * RAD_TO_DEG;
            lat2Out = y[1] * RAD_TO_DEG;

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