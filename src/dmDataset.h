#pragma once

#ifndef _DM_DATASET_
#define _DM_DATASET_

#include "dm_API.h"

#include "gdal_priv.h"
#include "cpl_conv.h"

class dmDataset : public dm_API
{

public:
    dmDataset();
    ~dmDataset();


    unsigned char * getRasterData(
        coord &topLeftOut, coord &botRightOut) const override;
    unsigned char * getRasterData(
        const coord topLeftIn, const coord botRightIn,
        coord &topLeftOut, coord &botRightOut) const override;
    bool openDataSet(const char* filename) override;
    void setSrcWkt(const char * wkt);
    void setDstWkt(const char * wkt);


private:
    static bool driversRegistered;
    static void registerGDALDrivers();

    const char * _srcWkt;
    const char * _dstWkt;
    GDALDataset * _srcDataset;
    GDALDataset * _dstDataset;


    void reprojectDataset();

};

#endif _DM_DATASET_