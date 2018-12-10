#pragma once

#ifndef _DM_DATASET_
#define _DM_DATASET_

#include "dm_API.h"

#include <vector>
#include "gdal_priv.h"
#include "cpl_conv.h"


typedef std::unique_ptr<char*[]> gdalUtilOpts;

class dmDataset : public dm_API
{

public:
    dmDataset(dmLogWriter* logWriter);
    ~dmDataset();


    bool getDatasetPixelDimensions(int &width, int &height) override;
    bool getDatasetExtremeValues(double& min, double& max) override;
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
    void setHillshadeZFactor(double zFactor) override;
    void setHillshadeScale(double scale) override;
    void setHillshadeAzimuth(double azimuth) override;
    void setHillshadeAltitude(double altitude) override;
    void setHillshadeCombined(bool combined) override;
    void setHillshadeMultidirectional(bool multidirectional) override;
    void setHillshadeAlpha(unsigned char alpha) override;

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
    std::pair<int, int> dmDataset::getRasterPixelOffsetAt(coord point, bool roundDownPixelOffset = true);
    dmExtent getRasterExtent(void);
    std::vector<std::string> getGdaldemOptionsVec();
    std::vector<std::string> getGdalwarpOptionsVec();
    gdalUtilOpts optionsVecToGdalUtilOpts(std::vector<std::string> &options);
    GDALDataset * reprojectDataset(GDALDataset *dsToReproject);
    GDALDataset * visualizeDataset(GDALDataset *dsToVisualize);

};

#endif _DM_DATASET_