#include "dmDataSet.h"

#include "gdalwarper.h"


dmDataset::dmDataset() :
    _srcDataset(NULL),
    _dstDataset(NULL)
{
    // GDALAllRegister(); // where to do this?
}


dmDataset::~dmDataset()
{
    if (_srcDataset)
        GDALClose(_srcDataset);

    if (_dstDataset)
        GDALClose(_dstDataset);
}


unsigned char * dmDataset::getRasterData(
    coord &topLeftOut, coord &botRightOut) const
{
    // TODO: implement
    return NULL;
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
    _srcDataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);

    if (_srcDataset)
        _srcWkt = GDALGetProjectionRef(_srcDataset);

    return false;
}

void dmDataset::reprojectDataset()
{
    // TODO: add GDALWarpOptions if needed (last argument of GDALAutoCreateWarpedVRT
    if (_srcDataset)
    {
        if (_dstDataset)
            GDALClose(_dstDataset);
        _dstDataset = (GDALDataset*)GDALAutoCreateWarpedVRT(_srcDataset, _srcWkt,
            _dstWkt, GRA_NearestNeighbour, 0.0, NULL);
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