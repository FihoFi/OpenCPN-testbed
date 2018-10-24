#pragma once

#ifndef _DM_DATASET_
#define _DM_DATASET_

#include "dm_API.h"

#include "gdal_priv.h"
#include "cpl_conv.h"

class dmDataset : public dm_API
{

public:
    dmDataset(dmLogWriter* logWriter);
    ~dmDataset();


    bool getDatasetPixelDimensions(int &width, int &height) override;
    bool getDatasetExtents(coord &topLeft, coord &botRight) override;
    bool setColourConfigurationFile(const char* filename, bool giveOwnership) override;
    bool setColourConfiguration(const char* fileContents, bool giveOwnership) override;
    bool setVisualizationScheme(DM_visualization visScheme) override;
    dmRasterImgData * getRasterData(
        coord &topLeftOut, coord &botRightOut) override;
    dmRasterImgData * getRasterData(
        const coord topLeftIn, const coord botRightIn,
        coord &topLeftOut, coord &botRightOut,
        int &imgWidth, int &imgHeight) override;
    bool openDataSet(const char* filename) override;
    void setSrcWkt(const char * wkt);
    void setDstWkt(const char * wkt);

    // setters for hillshade parameters
    bool setHillshadeZFactor(double zFactor) override;
    bool setHillshadeScale(double scale) override;
    bool setHillshadeAzimuth(double azimuth) override;
    bool setHillshadeAltitude(double altitude) override;
    bool setHillshadeCombined(bool combined) override;
    bool setHillshadeMultidirectional(bool multidirectional) override;

private:
    static bool driversRegistered;
    static void registerGDALDrivers();

    // hillshade parameters
    int _hillshadeParamZFactor;
    int _hillshadeParamScale;
    int _hillshadeParamAzimuth;
    int _hillshadeParamAltitude;
    bool _hillshadeParamCombined;
    bool _hillshadeParamMultidirectional;

    std::string _srcWkt;
    std::string _dstWkt;
    std::string _colorConfFilename;
    GDALDataset * _srcDataset;
    GDALDataset * _dstDataset;

    DM_visualization _visScheme;
    
    bool dstSrsToLatLon(double e, double n, coord &latLons);
    bool getCropExtents(coord topLeftIn, coord botRightIn,
        coord &topLeftOut, coord &botRightOut,
        int &pixOffsetX, int &pixOffsetY,
        int &imgWidth, int &imgHeight);
    GDALDataset * reprojectDataset(GDALDataset *dsToReproject);
    GDALDataset * visualizeDataset(GDALDataset *dsToVisualize);

};

#endif _DM_DATASET_