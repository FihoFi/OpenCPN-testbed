//#include "gdal_priv.h"
//#include "cpl_conv.h" // for CPLMalloc()
#include "dmDepthModelDrawer.h"

#include <proj_api.h>
#include "dm_API.h"
#include "dmDataset.h"

dmDepthModelDrawer::dmDepthModelDrawer()
    : rasterToDraw(NULL), modelState(UNSET)
{    }

dmDepthModelDrawer::~dmDepthModelDrawer()
{    delete rasterToDraw;    }

bool dmDepthModelDrawer::setDepthModelDataset(wxFileName &fileName)
{
    wxString    fileNameWxStr   = fileName.GetFullPath();
    std::string fileNameStr     = fileNameWxStr.ToStdString();
    const char* fileNameCharPtr = fileNameStr.c_str();

    bool success = dataset.openDataSet(fileNameCharPtr);
    if (success)
    {
        depthModelFileName = fileNameCharPtr;
        modelState = PROJECTION_OK; }
    else
    {
        wxLogMessage(_T("dmDepthModelDrawer::setDepthModelDataset openDataSet failed: ")+ fileNameStr);
    }
    return success;
}

bool dmDepthModelDrawer::applyChartAreaData(coord chartTopLeft, coord chartBotRight)
{
    if (modelState < FILE_SET/*PROJECTION_OK*/) { return false; }

    // TODO do the calculations for new wantedTopLeft, and wantedBotRight
    ////TODO CHANGE THESE DUMMY COORDINATES
    ////transformation  transf_f = PROJ;
    //gimmeLatLons(/*transf_f,*/ // From World Mercator to LatLon
    //    coords[0], coords[3], coords[2], coords[1],
    //    latMax, lonMin, latMin, lonMax);

    // TODO THIS MUST BE CROPPED SOMEHOW WITH CURRENT WINDOW RELATED VALUES
    //unsigned char*  oldRaster = rasterToDraw;   // save the old pointer for deletion
    if(!rasterToDraw)
    this->chartTopLeft  = chartTopLeft   /* + some offset to out of the window */;
    this->chartBotRight = chartBotRight /* + some offset to out of the window */;

    if(true)
    {
        try
        {
            rasterToDraw = dataset.getRasterData(//wantedTopLeft, wantedBotRight,
                imageTopLeft, imageBotRight);  // probably in latlons
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

        double crds[4];
        crds[0] = imageTopLeft.lon;  // lon (World mercator)
        crds[1] = imageBotRight.lat; // lat
        crds[2] = imageBotRight.lon; // lon
        crds[3] = imageTopLeft.lat;  // lat
        gimmeLatLons(WORLD_MERCATOR, crds[0], crds[3], crds[2], crds[1],
            imageTopLeft.lat, imageTopLeft.lon, imageBotRight.lat, imageBotRight.lon);
    }

    // TODO must make nicer; now crops just to minimum areas
    wantedTopLeft.lat  = (imageTopLeft.lat  < chartTopLeft.lat)  ? imageTopLeft.lat  : chartTopLeft.lat;
    wantedTopLeft.lon =  (imageTopLeft.lon  > chartTopLeft.lon)  ? imageTopLeft.lon  : chartTopLeft.lon;
    wantedBotRight.lat = (imageBotRight.lat > chartBotRight.lat) ? imageBotRight.lat : chartBotRight.lat;
    wantedBotRight.lon = (imageBotRight.lon < chartBotRight.lon) ? imageBotRight.lon : chartBotRight.lon;


    if (rasterToDraw)   // TODO does the getRasterData fail? how?
    {
        modelState = CHART_AREA_OK;
    }
    return true;
}

bool dmDepthModelDrawer::calculateDepthModelBitmap(PlugIn_ViewPort &vp)
{
    if (modelState < CHART_AREA_OK) { return false; }

    {
        wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - LoadFile failed: ") +
            depthModelFileName.GetName().ToStdString());
        return false;
    }

    // Get min, and max coordinates where the bitmap is to be drawn
    wxPoint r1, r2;
    GetCanvasPixLL(&vp, &r1, wantedTopLeft.lat,  wantedTopLeft.lon);   // up-left
    GetCanvasPixLL(&vp, &r2, wantedBotRight.lat, wantedBotRight.lon);  // low-right

    // Calculate dimensions of the picture
    int w = r2.x - r1.x; // max-min
    int h = r2.y - r1.y; // max-min

    if ((w > 10 && h > 10) && (w < 10000 && h < 10000))
    {
        // Generate the image with Dataset/GDAL
        wxImage* originalFromGDAL = new wxImage(w, h, rasterToDraw, false);
        rasterToDraw = NULL; // was freed by wxImage constructor

        bitmapTopLeftPositioningPoint = r1;
        bmp = wxBitmap(*originalFromGDAL);
        //bmp.SetMask(new wxMask(bmp, wxColour(255, 255, 255)));
        delete originalFromGDAL;

    }
   else
   {
       bmp = wxBitmap();
       wxLogMessage(_T("dmDepthModelDrawer::calculateDepthModelBitmap - dimension fail: w,h: ") +
           wxString::Format(_T("%i"), w)+ "," + wxString::Format(_T("%i"), h));
   }

    modelState = BITMAP_AVAILABLE;

    return true;
}


bool dmDepthModelDrawer::drawDepthChart(wxDC &dc/*, PlugIn_ViewPort &vp*/)
{
    if (modelState < CHART_AREA_OK/*BITMAP_AVAILABLE*/) { return false; }

    //wxString  fname = "C:\\OPENCPN_DATA\\UkiImg_wm.png";

    dc.DrawBitmap(bmp, bitmapTopLeftPositioningPoint, true);

    return true;
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