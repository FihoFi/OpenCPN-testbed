#include "dmDataset.h"

#include <stdio.h>
#include "gdal_utils.h"
#include "proj.h"

bool dmDataset::driversRegistered = false;

dmDataset::dmDataset() :
    _visScheme(HILLSHADE),
    _srcDataset(NULL),
    _dstDataset(NULL),
    _dstWkt("PROJCS[\"WGS 84 / World Mercator\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],PROJECTION[\"Mercator_1SP\"],PARAMETER[\"central_meridian\",0],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],AUTHORITY[\"EPSG\",\"3395\"],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH]]")
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
    int xSize, ySize;
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

    // read RGB channel
    int n = 0;
    while (n < 3 && n < bands.size())
    {
        bandData = (float*)CPLMalloc(sizeof(float)*xSize*ySize);
        bands[n]->RasterIO(GF_Read, 0, 0, xSize, ySize, bandData, xSize, ySize, GDT_Float32, 0, 0);

        for (int i = 0; i < xSize*ySize; i++)
        {
            imgData->rgb[3 * i + n] = (unsigned char)bandData[i];
        }
        CPLFree(bandData);
        n++;
    }

    // read alpha channel
    if (bands.size() > 3)
    {
        bandData = (float*)CPLMalloc(sizeof(float)*xSize*ySize);
        bands[3]->RasterIO(GF_Read, 0, 0, xSize, ySize, bandData, xSize, ySize, GDT_Float32, 0, 0);

        for (int i = 0; i < xSize*ySize; i++)
        {
            imgData->alpha[i] = (unsigned char)bandData[i];
        }
        CPLFree(bandData);
    }

    return imgData;
}

dmRasterImgData * dmDataset::getRasterData(int imgWidth, int imgHeight,
    const coord topLeftIn, const coord botRightIn,
    coord &topLeftOut, coord &botRightOut)
{
    // TODO: implement
    return NULL;
}

bool dmDataset::openDataSet(const char * filename)
{
    GDALDataset *reprojectedDs;

    if (_srcDataset)
        GDALClose(_srcDataset);

    _srcDataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);

    if (_srcDataset)
    {
        _srcWkt = GDALGetProjectionRef(_srcDataset);

        reprojectedDs = reprojectDataset(_srcDataset);

        if (!reprojectedDs)
            return false;

        _dstDataset = visualizeDataset(reprojectedDs);

        if (!_dstDataset)
            return false;

        _dstWkt = GDALGetProjectionRef(_dstDataset);

        return true;
    }

    return false;
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

GDALDataset * dmDataset::reprojectDataset(GDALDataset *dsToReproject)
{
    if (dsToReproject)
    {
        int err = 0;
        GDALDataset *warpedDS;

        char *warpOpts[] = { (char*)"-t_srs", (char *)_dstWkt.c_str(),
                             (char*)"-r",     (char*)"max",
                             NULL };

        // coordinate system reprojection
        GDALWarpAppOptions *psWarpOptions = GDALWarpAppOptionsNew(warpOpts, NULL);
        warpedDS = (GDALDataset*)GDALWarp(".\\warped_ds.tif", NULL, 1, (GDALDatasetH*)&_srcDataset, psWarpOptions, &err);

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

    GDALDEMProcessingOptions * gdaldemOptions = GDALDEMProcessingOptionsNew(nullptr, nullptr);

    char *colorReliefOptions[] = {
        const_cast<char *>("-alpha"),
        const_cast<char *>("-nearest_color_entry"),
        nullptr };  // The last entry must be a nullptr
    GDALDEMProcessingOptions * gdaldemOptionsColorRelief = GDALDEMProcessingOptionsNew(colorReliefOptions, nullptr);

    switch (_visScheme)
    {
    case HILLSHADE:
        resultDs = (GDALDataset*)GDALDEMProcessing(".\\temp_ds.tif", dsToVisualize, "hillshade", NULL, gdaldemOptions, &err);
        GDALClose(dsToVisualize);
        break;

    case COLOR_RELIEF:
        resultDs = (GDALDataset*)GDALDEMProcessing(".\\temp_ds.tif", dsToVisualize, "color-relief", _colorConfFilename.c_str(), gdaldemOptionsColorRelief, &err);
        GDALClose(dsToVisualize);
        break;

    case NONE:
    default:
        resultDs = dsToVisualize;
        break;
    }

    // clean up
    GDALDEMProcessingOptionsFree(gdaldemOptions);
    GDALDEMProcessingOptionsFree(gdaldemOptionsColorRelief);

    if (err)
        return NULL;

    return resultDs;
}

void dmDataset::setSrcWkt(const char * wkt)
{
    _srcWkt = wkt;
}

void dmDataset::setDstWkt(const char * wkt)
{
    _dstWkt  = wkt;
}
