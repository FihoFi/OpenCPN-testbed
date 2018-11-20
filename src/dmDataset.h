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
    void setSrcWkt(const char * wkt);
    void setDstWkt(const char * wkt);
    bool setColourConfigurationFile(const char* filename, bool giveOwnership) override;
    bool setColourConfiguration(const char* fileContents, bool giveOwnership) override;
    void setTempFolderPath(std::string tempFolderPath);
    bool setVisualizationScheme(DM_visualization visScheme) override;
    dmRasterImgData * getRasterData(
        coord &topLeftOut, coord &botRightOut) override;
    dmRasterImgData * getRasterData(
        const coord topLeftIn, const coord botRightIn,
        coord &topLeftOut, coord &botRightOut,
        int &imgWidth, int &imgHeight) override;

    /** Gets the depth value in the reprojected dataset
     * at the given location.
     * 
     * @param[in] point location coordinates in WM
     *
     * @return depth value at the given location
     */
    float getDepthAt(coord point);

    bool openDataSet(const char* filename) override;
    bool dstSrsToLatLon(coord dstSrsIn, coord &latLonOut) override;
    bool dstSrsToLatLon(dmExtent dstSrsIn, dmExtent &latLonOut) override;
    bool latLonToDstSrs(coord latLonIn, coord &dstSrsOut) override;
    bool latLonToDstSrs(dmExtent latLonIn, dmExtent &dstSrsOut) override;

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
    double _hillshadeParamZFactor;
    double _hillshadeParamScale;
    double _hillshadeParamAzimuth;
    double _hillshadeParamAltitude;
    bool _hillshadeParamCombined;
    bool _hillshadeParamMultidirectional;
    unsigned char _hillshadeAlpha;

    std::string _srcWkt;
    std::string _dstWkt;
    std::string _colorConfFilename;
    std::string _tempFolderPath;
    GDALDataset * _srcDataset;
    GDALDataset * _reprojectedDataset;
    GDALDataset * _dstDataset;
    double _geoTransform[6];
    dmRasterImgData * _imgData;

    DM_visualization _visScheme;

    bool allocateImgDataMemory();
    bool getCropExtents(coord topLeftIn, coord botRightIn,
        coord &topLeftOut, coord &botRightOut,
        int &pixOffsetX, int &pixOffsetY,
        int &imgWidth, int &imgHeight);
    std::pair<int, int> dmDataset::getRasterPixelOffsetAt(coord point, bool pixelTopLeftCorner = true);
    dmExtent getRasterExtent(void);
    std::vector<std::string> getGdaldemOptionsVec();
    GDALDataset * reprojectDataset(GDALDataset *dsToReproject);
    GDALDataset * visualizeDataset(GDALDataset *dsToVisualize);

};

#endif _DM_DATASET_