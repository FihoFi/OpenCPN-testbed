#include "dmDataset.h"

#include <stdio.h>
#include <algorithm>
#include "gdal_utils.h"
#include "proj.h"

bool dmDataset::driversRegistered = false;

dmDataset::dmDataset(dmLogWriter* logWriter) :
    dm_API(logWriter),
    _visScheme(HILLSHADE),
    _srcDataset(NULL),
    _dstDataset(NULL),
    _dstWkt("PROJCS[\"WGS 84 / World Mercator\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],PROJECTION[\"Mercator_1SP\"],PARAMETER[\"central_meridian\",0],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],AUTHORITY[\"EPSG\",\"3395\"],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH]]"),
    _hillshadeParamAzimuth(315.),
    _hillshadeParamAltitude(45.),
    _hillshadeParamMultidirectional(false)
{
    dmDataset::registerGDALDrivers();
}


dmDataset::~dmDataset()
{
    if (_srcDataset)
        GDALClose(_srcDataset);

    if (_dstDataset)
        GDALClose(_dstDataset);

    remove(".\\warped_ds.tif");
    remove(".\\temp_ds.tif");
}

bool dmDataset::getDatasetPixelDimensions(int &width, int &height)
{
    if (_dstDataset)
    {
        width = _dstDataset->GetRasterXSize();
        height = _dstDataset->GetRasterYSize();

        return true;
    }

    return false;
}

bool dmDataset::getDatasetExtents(coord &topLeft, coord &botRight)
{
    double geoTransform[6];

    if (GDALGetGeoTransform(_dstDataset, geoTransform) != CPLErr::CE_None)
        return false;

    double xSize = _dstDataset->GetRasterXSize();
    double ySize = _dstDataset->GetRasterYSize();

    topLeft.east = geoTransform[0];
    topLeft.north = geoTransform[3];
    botRight.east = geoTransform[0] + xSize * geoTransform[1] + ySize * geoTransform[2];
    botRight.north = geoTransform[3] + xSize * geoTransform[4] + ySize * geoTransform[5];

    return true;

}

bool dmDataset::setColourConfigurationFile(const char* filename, bool giveOwnership)
{
    if (filename)
    {
        _colorConfFilename = filename;

        if (giveOwnership)
        {
            // is this the right kind of deallocation here? how about e.g. delete, CPLFree?
            free(const_cast<char*>(filename));
        }
    }

    return true;
}

bool dmDataset::setColourConfiguration(const char* fileContents, bool giveOwnership)
{
    // TODO: implement if needed

    if (fileContents)
    {
        if (giveOwnership)
        {
            // TODO: is this the right kind of deallocation here? how about e.g. delete, CPLFree?
            free(const_cast<char*>(fileContents));
        }
    }
    return false;
}

bool dmDataset::setVisualizationScheme(DM_visualization visScheme)
{
    _visScheme = visScheme;
    if (visScheme == NONE || visScheme == HILLSHADE || visScheme == COLOR_RELIEF)
        return true;
    else
        return false;
}

dmRasterImgData * dmDataset::getRasterData(
    coord &topLeftOut, coord &botRightOut)
{
    int xSize, ySize, n;
    float *bandData;
    dmRasterImgData *imgData;

    if (!_dstDataset)
        return NULL;
    
    GDALDataset::Bands bands = _dstDataset->GetBands();
    if (bands.size() < 1)
        return NULL;

    if (!getDatasetExtents(topLeftOut, botRightOut))
        return NULL;

    xSize = bands[0]->GetXSize();
    ySize = bands[0]->GetYSize();

    imgData = new dmRasterImgData();
    imgData->rgb = new unsigned char[3 * xSize*ySize];
    imgData->alpha = new unsigned char[xSize*ySize];

    n = 0;

    // read RGB channel
    while (n < 3)
    {
        if (_visScheme == HILLSHADE)
            bands[0]->RasterIO(GF_Read, 0, 0, xSize, ySize, imgData->rgb + n, xSize, ySize, GDT_Byte, 3, 3 * xSize);
        else if ((_visScheme == COLOR_RELIEF || _visScheme == NONE) && n < bands.size())
            bands[n]->RasterIO(GF_Read, 0, 0, xSize, ySize, imgData->rgb + n, xSize, ySize, GDT_Byte, 3, 3*xSize);

        n++;
    }

    // read alpha channel (assumed to be the in the last raster band)
    if (bands.size() > 1)
        bands[bands.size()-1]->RasterIO(GF_Read, 0, 0, xSize, ySize, imgData->alpha, xSize, ySize, GDT_Byte, 0, 0);

    return imgData;
}

dmRasterImgData * dmDataset::getRasterData(
    const coord topLeftIn, const coord botRightIn,
    coord &topLeftOut, coord &botRightOut,
    int &imgWidth, int &imgHeight)
{
    int n;
    int imgOffsetX, imgOffsetY;
    dmRasterImgData *imgData;

    getCropExtents(topLeftIn, botRightIn,
        topLeftOut, botRightOut,
        imgOffsetX, imgOffsetY,
        imgWidth, imgHeight);

    if (!_dstDataset)
        return NULL;

    GDALDataset::Bands bands = _dstDataset->GetBands();
    if (bands.size() < 1)
        return NULL;
    
    imgData = new dmRasterImgData();
    imgData->rgb = new unsigned char[3 * imgWidth*imgHeight];
    imgData->alpha = new unsigned char[imgWidth*imgHeight];

    n = 0;

    // read RGB channel
    while (n < 3)
    {
        if (_visScheme == HILLSHADE)
            bands[0]->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, imgData->rgb + n, imgWidth, imgHeight, GDT_Byte, 3, 3 * imgWidth);
        else if ((_visScheme == COLOR_RELIEF || _visScheme == NONE) && n < bands.size())
            bands[n]->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, imgData->rgb + n, imgWidth, imgHeight, GDT_Byte, 3, 3 * imgWidth);

        n++;
    }

    // read alpha channel (assumed to be the in the last raster band)
    if (bands.size() > 1)
        bands[bands.size() - 1]->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, imgData->alpha, imgWidth, imgHeight, GDT_Byte, 0, 0);

    return imgData;

    return NULL;
}

bool dmDataset::openDataSet(const char * filename)
{
    GDALDataset *reprojectedDs;

    if (_srcDataset)
        GDALClose(_srcDataset);

    if (_dstDataset)
        GDALClose(_dstDataset);

    _srcDataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);

    if (_srcDataset)
    {
        _srcWkt = GDALGetProjectionRef(_srcDataset);

        reprojectedDs = visualizeDataset(_srcDataset);

        if (!reprojectedDs)
            return false;

        _dstDataset = reprojectDataset(reprojectedDs);

        if (reprojectedDs)
            GDALClose(reprojectedDs);

        if (!_dstDataset)
            return false;

        if (_visScheme == HILLSHADE)
            applyHillshadeAlphaMask(_dstDataset);

        _dstWkt = GDALGetProjectionRef(_dstDataset);

        return true;
    }

    return false;
}

void dmDataset::setSrcWkt(const char * wkt)
{
    _srcWkt = wkt;
}

void dmDataset::setDstWkt(const char * wkt)
{
    _dstWkt = wkt;
}


bool dmDataset::setHillshadeZFactor(double zFactor)
{
    _hillshadeParamZFactor = zFactor;

    return true;
}

bool dmDataset::setHillshadeScale(double scale)
{
    _hillshadeParamScale = scale;

    return true;
}

bool dmDataset::setHillshadeAzimuth(double azimuth)
{
    _hillshadeParamAzimuth = azimuth;

    return true;
}

bool dmDataset::setHillshadeAltitude(double altitude)
{
    _hillshadeParamAltitude = altitude;

    return true;
}

bool dmDataset::setHillshadeCombined(bool combined)
{
    _hillshadeParamCombined = combined;

    return true;
}

bool dmDataset::setHillshadeMultidirectional(bool multidirectional)
{
    _hillshadeParamMultidirectional = multidirectional;

    return true;
}


/* private */

bool dmDataset::applyHillshadeAlphaMask(GDALDataset * ds)
{
    unsigned char * alpha;
    int xSize, ySize;
    GDALRasterBand *band;

    if (!ds)
        return false;

    GDALDataset::Bands bands = ds->GetBands();

    if (bands.size() < 2)
        return false;

    band = bands[1];

    xSize = band->GetXSize();
    ySize = band->GetYSize();

    alpha = new unsigned char[xSize*ySize];

    band->RasterIO(GF_Read, 0, 0, xSize, ySize, alpha, xSize, ySize, GDT_Byte, 0, 0);

    for (int i = 0; i < xSize*ySize; i++)
    {
        if (alpha[i] != 0) // max alpha channel value
            alpha[i] = 128;
    }

    band->RasterIO(GF_Write, 0, 0, xSize, ySize, alpha, xSize, ySize, GDT_Byte, 0, 0);

    delete[] alpha;
}

void dmDataset::registerGDALDrivers()
{
    if (!driversRegistered)
    {
        GDALAllRegister();
        driversRegistered = true;
    }
}

bool dmDataset::dstSrsToLatLon(double n, double e, coord &latLons)
{
    PJ *projection;
    PJ_COORD from, to;
    OGRSpatialReference osr(GDALGetProjectionRef(_dstDataset));
    char *projStr;
    osr.exportToProj4(&projStr);
    projection = proj_create(PJ_DEFAULT_CTX, projStr);

    CPLFree(projStr);
    
    if (!projection)
        return false;

    from = proj_coord(n, e, 0, 0);
    
    to = proj_trans(projection, PJ_INV, from);

    latLons.north = proj_todeg(to.enu.n);
    latLons.east = proj_todeg(to.enu.e);

    /* Clean up */
    proj_destroy(projection);

    return true;
}

bool dmDataset::getCropExtents(coord topLeftIn, coord botRightIn,
    coord &topLeftOut, coord &botRightOut,
    int &imgOffsetX, int &imgOffsetY,
    int &imgWidth, int &imgHeight)
{
    int xSize = _dstDataset->GetRasterXSize();
    int ySize = _dstDataset->GetRasterYSize();
    double geoTransform[6];

    if (_dstDataset->GetGeoTransform(geoTransform) != CE_None)
    {
        return false;
    }

    coord topLeftRaster(geoTransform[3],
                        geoTransform[0]);
    coord botRightRaster(geoTransform[3] + xSize * geoTransform[4] + ySize * geoTransform[5],
                         geoTransform[0] + xSize * geoTransform[1] + ySize * geoTransform[2]);

    if (topLeftIn.east < topLeftRaster.east)
    {
        imgOffsetX = 0;
        topLeftOut.east = topLeftRaster.east;
    }
    else
    {
        imgOffsetX = std::floor((topLeftIn.east - topLeftRaster.east) / geoTransform[1]);
        topLeftOut.east = geoTransform[0] + imgOffsetX * geoTransform[1];
    }

    if (topLeftIn.north < topLeftRaster.east)
    {
        imgOffsetY = 0;
        topLeftOut.north = topLeftRaster.north;
    }
    else
    {
        imgOffsetY = std::floor((topLeftIn.north - topLeftRaster.north) / geoTransform[5]);
        topLeftOut.north = geoTransform[3] + imgOffsetY * geoTransform[5];
    }

    if (botRightIn.east > botRightRaster.east)
    {
        imgWidth = xSize - imgOffsetX;
        botRightOut.east = botRightRaster.east;
    }
    else
    {
        imgWidth = std::ceil((botRightIn.east - topLeftRaster.east) / geoTransform[1]) - imgOffsetX;
        botRightOut.east = topLeftOut.east + imgWidth*geoTransform[1];
    }

    if (botRightIn.north > botRightRaster.north)
    {
        imgHeight = ySize - imgOffsetY;
        botRightOut.north = botRightRaster.north;
    }
    else
    {
        imgHeight = std::ceil((botRightIn.north - topLeftRaster.north) / geoTransform[5]) - imgOffsetY;
        botRightOut.north = topLeftOut.north + imgHeight * geoTransform[5];
    }
}


std::vector<std::string> dmDataset::getGdaldemOptionsVec()
{
    std::vector<std::string> optionsVec;

    switch (_visScheme)
    {
    case HILLSHADE:
        optionsVec.push_back("-az");
        optionsVec.push_back(std::to_string(_hillshadeParamAzimuth));
        optionsVec.push_back("-alt");
        optionsVec.push_back(std::to_string(_hillshadeParamAltitude));
        if (_hillshadeParamMultidirectional)
            optionsVec.push_back("-multidirectional");
        break;
    case COLOR_RELIEF:
        optionsVec.push_back("-alpha");
        optionsVec.push_back("-nearest_color_entry");
        break;
    case NONE:
    default:
        break;
    }

    return optionsVec;
}

GDALDataset * dmDataset::reprojectDataset(GDALDataset *dsToReproject)
{
    if (dsToReproject)
    {
        int err = 0;
        GDALDataset *warpedDS;

        char *warpOpts[] = { (char*)"-t_srs", (char *)_dstWkt.c_str(),
                             (char*)"-r",     (char*)"max",
                             (_visScheme == HILLSHADE) ? ((char*)"-dstalpha") : ((char*)"-nosrcalpha"),
                             NULL };

        // coordinate system reprojection
        GDALWarpAppOptions *psWarpOptions = GDALWarpAppOptionsNew(warpOpts, NULL);
        warpedDS = (GDALDataset*)GDALWarp(".\\warped_ds.tif", NULL, 1, (GDALDatasetH*)&dsToReproject, psWarpOptions, &err);

        // clean up
        GDALWarpAppOptionsFree(psWarpOptions);

        if (err)
            return NULL;

        return warpedDS;
    }

    return NULL;
}

GDALDataset * dmDataset::visualizeDataset(GDALDataset *dsToVisualize)
{
    int err = 0;
    GDALDataset *resultDs;

    // put gdaldem processing flags into a c string array
    std::vector<std::string> optionsVec = getGdaldemOptionsVec();
    char** optionsArr = new char*[optionsVec.size() + 1];
    for (int i = 0; i<optionsVec.size(); i++)
    {
        optionsArr[i] = (char*)optionsVec[i].c_str();
    }
    optionsArr[optionsVec.size()] = nullptr;

    GDALDEMProcessingOptions * gdaldemOptions = GDALDEMProcessingOptionsNew(optionsArr, nullptr);

    switch (_visScheme)
    {
    case HILLSHADE:
        resultDs = (GDALDataset*)GDALDEMProcessing(".\\temp_ds.tif", dsToVisualize, "hillshade", NULL, gdaldemOptions, &err);
        break;

    case COLOR_RELIEF:
        resultDs = (GDALDataset*)GDALDEMProcessing(".\\temp_ds.tif", dsToVisualize, "color-relief", _colorConfFilename.c_str(), gdaldemOptions, &err);
        break;

    case NONE:
    default:
        resultDs = dsToVisualize;
        break;
    }

    // clean up
    GDALDEMProcessingOptionsFree(gdaldemOptions);

    if (err)
        return NULL;

    return resultDs;
}
