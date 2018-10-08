#include "dmDataset.h"

#include <stdio.h>
#include "gdal_utils.h"
//#include "gdalwarper.h"

bool dmDataset::driversRegistered = false;

dmDataset::dmDataset() :
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


unsigned char * dmDataset::getRasterData(
    coord &topLeftOut, coord &botRightOut) const
{
    GDALRasterBand *band = _dstDataset->GetRasterBand(1);

    int xSize, ySize;
    int *bandData;
    unsigned char *imgData;

    xSize = band->GetXSize();
    ySize = band->GetYSize();
    imgData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * xSize*ySize);
    bandData = (int*)CPLMalloc(sizeof(int)*xSize*ySize);
    band->RasterIO(GF_Read, 0, 0, xSize, ySize, bandData, xSize, ySize, GDT_Byte, 0, 0);

    for (int i = 0; i < xSize*ySize; i++)
    {
        imgData[3 * i] = (unsigned char)bandData[i];
        imgData[3 * i + 1] = (unsigned char)bandData[i];
        imgData[3 * i + 2] = (unsigned char)bandData[i];
    }
    CPLFree(bandData);

    return imgData;
}

unsigned char * dmDataset::getRasterData(
    const coord topLeftIn, const coord botRightIn,
    coord &topLeftOut, coord &botRightOut) const
{
    // TODO: implement
    return NULL;
}

bool dmDataset::openDataSet(const char * filename)
{
    if (_srcDataset)
        GDALClose(_srcDataset);

    _srcDataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);

    if (_srcDataset)
        _srcWkt = GDALGetProjectionRef(_srcDataset);

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

void dmDataset::reprojectDataset()
{
    // TODO: add GDALWarpOptions if needed (last argument of GDALAutoCreateWarpedVRT)
    if (_srcDataset)
    {
        if (_dstDataset)
            GDALClose(_dstDataset);

        int err = 0;
        GDALDataset *warpedDS;
        static char dstWKT[1000] = "PROJCS[\"WGS 84 / Pseudo - Mercator\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]],PROJECTION[\"Mercator_1SP\"],PARAMETER[\"central_meridian\",0],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH],EXTENSION[\"PROJ4\",\"+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs\"],AUTHORITY[\"EPSG\",\"3857\"]]";
        //char dstProjStr[1000] = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs";
        static char* warpOpts[] = { (char*)"-t_srs", dstWKT , NULL };

        // TODO: use Max resampling algorithm
        GDALWarpAppOptions *psWarpOptions = GDALWarpAppOptionsNew(warpOpts, NULL);
        //GDALWarpAppOptions *psWarpOptions = GDALWarpAppOptionsNew(NULL, NULL);

        warpedDS = (GDALDataset*)GDALWarp(".\\warped_ds.tif", NULL, 1, (GDALDatasetH*)&_srcDataset, psWarpOptions, &err);

        GDALWarpAppOptionsFree(psWarpOptions);

        GDALDEMProcessingOptions* psOptionsToFree = GDALDEMProcessingOptionsNew(nullptr, nullptr);
        //dstDS = (GDALDataset*)GDALDEMProcessing("C:\\livi_data\\test.tif", srcDS, "hillshade", NULL, NULL, &err);
        _dstDataset = (GDALDataset*)GDALDEMProcessing(".\\temp_ds.tif", warpedDS, "hillshade", NULL, psOptionsToFree, &err);

        GDALClose(warpedDS);
        GDALDEMProcessingOptionsFree(psOptionsToFree);
    }
}

void dmDataset::setSrcWkt(const char * wkt)
{
    _srcWkt = wkt;
}

void dmDataset::setDstWkt(const char * wkt)
{
    _dstWkt  = wkt;
}