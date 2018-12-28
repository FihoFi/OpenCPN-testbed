/******************************************************************************
*
* Project:  OpenCPN
* Purpose:  LIVI_Depth_model Plugin
* Author:   Sitowise & LIVI, based on DR_pi example plugin code
*
***************************************************************************
*   Copyright (C) 2018 by Sitowise & LIVI                                 *
*   $EMAIL$                                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************
*/
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
    bool visualizeDataSet() override;
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