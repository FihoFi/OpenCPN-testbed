#include "dmDataset.h"

#include <stdio.h>
#include "gdal_utils.h"
#include "proj.h"

bool dmDataset::driversRegistered = false;

dmDataset::dmDataset() :
    _visScheme(HILLSHADE),
    _srcDataset(NULL),
    _dstDataset(NULL)
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

void dmDataset::setVisualizationScheme(DM_visualization visScheme)
{
    _visScheme = visScheme;
}

unsigned char * dmDataset::getRasterData(
    coord &topLeftOut, coord &botRightOut)
{
    if (!_dstDataset)
        return NULL;

    GDALRasterBand *band;
    if (_dstDataset->GetRasterCount() > 0)
        band = _dstDataset->GetRasterBand(1);
    else
        return NULL;

    int xSize, ySize;
    float *bandData;
    unsigned char *imgData;

    if (!getDatasetExtents(topLeftOut, botRightOut))
        return NULL;

    xSize = band->GetXSize();
    ySize = band->GetYSize();

    imgData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * xSize*ySize);
    bandData = (float*)CPLMalloc(sizeof(float)*xSize*ySize);
    band->RasterIO(GF_Read, 0, 0, xSize, ySize, bandData, xSize, ySize, GDT_Float32, 0, 0);

    for (int i = 0; i < xSize*ySize; i++)
    {
        imgData[3 * i] = (unsigned char)bandData[i];
        imgData[3 * i + 1] = (unsigned char)bandData[i];
        imgData[3 * i + 2] = (unsigned char)bandData[i];
    }
    CPLFree(bandData);

     return imgData;
}

unsigned char * dmDataset::getRasterData(int imgWidth, int imgHeight,
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
        // TODO: take this from class variable/config?
        std::string dstWKT = "PROJCS[\"WGS 84 / Pseudo - Mercator\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]],PROJECTION[\"Mercator_1SP\"],PARAMETER[\"central_meridian\",0],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH],EXTENSION[\"PROJ4\",\"+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs\"],AUTHORITY[\"EPSG\",\"3857\"]]";
        char *warpOpts[] = { (char*)"-t_srs", (char *)dstWKT.c_str(),
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

    switch (_visScheme)
    {
    case HILLSHADE:
        resultDs = (GDALDataset*)GDALDEMProcessing(".\\temp_ds.tif", dsToVisualize, "hillshade", NULL, gdaldemOptions, &err);
        GDALClose(dsToVisualize);
        break;

    case COLOR_RELIEF:
        resultDs = (GDALDataset*)GDALDEMProcessing(".\\temp_ds.tif", dsToVisualize, "color-relief", _colorConfFilename.c_str(), gdaldemOptions, &err);
        GDALClose(dsToVisualize);
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

void dmDataset::setSrcWkt(const char * wkt)
{
    _srcWkt = wkt;
}

void dmDataset::setDstWkt(const char * wkt)
{
    _dstWkt  = wkt;
}