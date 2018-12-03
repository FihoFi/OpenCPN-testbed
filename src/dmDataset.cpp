#include "dmDataset.h"

#include <stdio.h>
#include <algorithm>
#include "gdal_utils.h"
#include "proj.h"

#include "dmExtent.h"

bool dmDataset::driversRegistered = false;

const std::string visFileName = "visualized_ds.tif";
const std::string warpedFileName = "warped_ds.tif";

dmDataset::dmDataset(dmLogWriter* logWriter) :
    dm_API(logWriter),
    _visScheme(HILLSHADE),
    _srcDataset(nullptr),
    _dstDataset(nullptr),
    _dstWkt("PROJCS[\"WGS 84 / World Mercator\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],PROJECTION[\"Mercator_1SP\"],PARAMETER[\"central_meridian\",0],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],AUTHORITY[\"EPSG\",\"3395\"],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH]]"),
    _tempFolderPath(".\\"),
    _imgData(nullptr),
    _hillshadeParamAzimuth(315.),
    _hillshadeParamAltitude(45.),
    _hillshadeParamMultidirectional(false),
    _hillshadeAlpha(128)
{
    dmDataset::registerGDALDrivers();
}


dmDataset::~dmDataset()
{
    if (_srcDataset)
        GDALClose(_srcDataset);

    if (_dstDataset)
        GDALClose(_dstDataset);

    remove((_tempFolderPath + visFileName).c_str());
    remove((_tempFolderPath + warpedFileName).c_str());
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

void dmDataset::setSrcWkt(const char * wkt)
{
    _srcWkt = wkt;
}

void dmDataset::setDstWkt(const char * wkt)
{
    _dstWkt = wkt;
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

void dmDataset::setTempFolderPath(std::string tempFolderPath)
{
    _tempFolderPath = tempFolderPath + "\\";
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
    int imgWidth, imgHeight;
    coord topLeftRaster, botRightRaster;

    if (!getDatasetExtents(topLeftRaster, botRightRaster))
        return NULL;

    return getRasterData(topLeftRaster, botRightRaster,
        topLeftOut, botRightOut, imgWidth, imgHeight);
}

dmRasterImgData * dmDataset::getRasterData(
    const coord topLeftIn, const coord botRightIn,
    coord &topLeftOut, coord &botRightOut,
    int &imgWidth, int &imgHeight)
{
    unsigned int n;
    int imgOffsetX, imgOffsetY;

    if (!_dstDataset)
        return NULL;

    getCropExtents(topLeftIn, botRightIn,
        topLeftOut, botRightOut,
        imgOffsetX, imgOffsetY,
        imgWidth, imgHeight);

    GDALDataset::Bands bands = _dstDataset->GetBands();
    if (bands.size() < 1)
        return NULL;

    n = 0;

    // read RGB channel
    while (n < 3)
    {
        if (_visScheme == HILLSHADE)
            bands[0]->RasterIO(GF_Read, imgOffsetX, imgOffsetY, imgWidth, imgHeight, _imgData->rgb + n, imgWidth, imgHeight, GDT_Byte, 3, 3 * imgWidth);
        else if ((_visScheme == COLOR_RELIEF || _visScheme == NONE) && n < bands.size())
            bands[n]->RasterIO(GF_Read, imgOffsetX, imgOffsetY, imgWidth, imgHeight, _imgData->rgb + n, imgWidth, imgHeight, GDT_Byte, 3, 3 * imgWidth);

        n++;
    }

    // read alpha channel (assumed to be the in the last raster band)
    bands[bands.size() - 1]->RasterIO(GF_Read, imgOffsetX, imgOffsetY, imgWidth, imgHeight, _imgData->alpha, imgWidth, imgHeight, GDT_Byte, 0, 0);

    // this is for e.g. hillshade; there is no alpha band in the raster after
    // gdaldem hillshade, so it has to be deduced from the image data
    if (bands.size() == 1)
    {
        std::transform(_imgData->alpha, _imgData->alpha + imgWidth*imgHeight, _imgData->alpha,
            [=](auto pixel) {
                if (pixel != 0)
                    return _hillshadeAlpha;
                return pixel;
        });
    }

    return _imgData;
}

float dmDataset::getDepthAt(coord point)
{
    float depth;
    std::pair<int, int> offsets = getRasterPixelOffsetAt(point);
    
    int x = offsets.first;
    int y = offsets.second;

    GDALDataset::Bands bands = _reprojectedDataset->GetBands();

    if (bands.size() == 0)
        return 0.;

    bands[0]->RasterIO(GF_Read, x, y, 1, 1, &depth, 1, 1, GDT_Float32, 0, 0);

    return depth;
}

bool dmDataset::openDataSet(const char * filename)
{
    if (_srcDataset)
    {
        GDALClose(_srcDataset);
        _srcDataset = NULL;
    }

    if (_dstDataset)
    {
        GDALClose(_dstDataset);
        _dstDataset = NULL;
    }

    _srcDataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);

    if (!_srcDataset)
        return false;

    _srcWkt = _srcDataset->GetProjectionRef();

    _reprojectedDataset = reprojectDataset(_srcDataset);

    if (!_reprojectedDataset)
        return false;

    _dstDataset = visualizeDataset(_reprojectedDataset);

    if (!_dstDataset)
        return false;

    if (!allocateImgDataMemory())
        return false;

    if (_dstDataset->GetGeoTransform(_geoTransform) != CE_None)
        return false;

    _dstWkt = _dstDataset->GetProjectionRef();

    return true;
}


void dmDataset::setHillshadeZFactor(double zFactor)
{
    _hillshadeParamZFactor = zFactor;
}

void dmDataset::setHillshadeScale(double scale)
{
    _hillshadeParamScale = scale;
}

void dmDataset::setHillshadeAzimuth(double azimuth)
{
    _hillshadeParamAzimuth = azimuth;
}

void dmDataset::setHillshadeAltitude(double altitude)
{
    _hillshadeParamAltitude = altitude;
}

void dmDataset::setHillshadeCombined(bool combined)
{
    _hillshadeParamCombined = combined;
}

void dmDataset::setHillshadeMultidirectional(bool multidirectional)
{
    _hillshadeParamMultidirectional = multidirectional;
}


/* private */

// static

void dmDataset::registerGDALDrivers()
{
    if (!driversRegistered)
    {
        GDALAllRegister();
        driversRegistered = true;
    }
}

// non-static

bool dmDataset::allocateImgDataMemory()
{
    if (_imgData)
    {
        delete _imgData;
        _imgData = NULL;
    }

    if (!_dstDataset)
        return false;

    _imgData = new dmRasterImgData();

    int xSize = _dstDataset->GetRasterXSize();
    int ySize = _dstDataset->GetRasterYSize();

    _imgData->rgb = new unsigned char[3 * xSize*ySize];
    _imgData->alpha = new unsigned char[xSize*ySize];

    return true;
}

std::pair<int, int> dmDataset::getRasterPixelOffsetAt(coord point, bool roundDownPixelOffset)
{
    double croppedEast, croppedNorth;
    dmExtent rasterExtent = getRasterExtent();
    double offsetX, offsetY;
    int pxOffsetX, pxOffsetY;

    croppedEast = std::max(point.east, rasterExtent.topLeft.east);
    croppedEast = std::min(croppedEast, rasterExtent.botRight.east);
    croppedNorth = std::min(point.north, rasterExtent.topLeft.north);
    croppedNorth = std::max(croppedNorth, rasterExtent.botRight.north);

    offsetX = std::abs((croppedEast - rasterExtent.topLeft.east) / _geoTransform[1]);
    offsetY = std::abs((croppedNorth - rasterExtent.topLeft.north) / _geoTransform[5]);

    if (roundDownPixelOffset)
    {
        pxOffsetX = std::max((int)std::floor(offsetX), 0);
        pxOffsetY = std::max((int)std::floor(offsetY), 0);
    }
    else
    {
        pxOffsetX = std::min((int)std::ceil(offsetX), _reprojectedDataset->GetRasterXSize());
        pxOffsetY = std::min((int)std::ceil(offsetY), _reprojectedDataset->GetRasterYSize());
    }

    return std::pair<int, int>(pxOffsetX, pxOffsetY);
}

dmExtent dmDataset::getRasterExtent(void)
{
    int xSize = _dstDataset->GetRasterXSize();
    int ySize = _dstDataset->GetRasterYSize();

    coord topLeftRaster(_geoTransform[3],
        _geoTransform[0]);
    coord botRightRaster(_geoTransform[3] + xSize * _geoTransform[4] + ySize * _geoTransform[5],
        _geoTransform[0] + xSize * _geoTransform[1] + ySize * _geoTransform[2]);

    return dmExtent(topLeftRaster, botRightRaster);
}

bool dmDataset::dstSrsToLatLon(coord dstSrsIn, coord &latLonOut)
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

    from = proj_coord(dstSrsIn.east, dstSrsIn.north, 0, 0);
    
    to = proj_trans(projection, PJ_INV, from);

    latLonOut.north = proj_todeg(to.enu.n);
    latLonOut.east = proj_todeg(to.enu.e);

    /* Clean up */
    proj_destroy(projection);

    return true;
}

bool dmDataset::dstSrsToLatLon(dmExtent dstSrsIn, dmExtent &latLonOut)
{
    bool success = true;

    success &= dstSrsToLatLon(dstSrsIn.topLeft, latLonOut.topLeft);
    success &= dstSrsToLatLon(dstSrsIn.botRight, latLonOut.botRight);

    return success;
}

bool dmDataset::getCropExtents(coord topLeftIn, coord botRightIn,
    coord &topLeftOut, coord &botRightOut,
    int &imgOffsetX, int &imgOffsetY,
    int &imgWidth, int &imgHeight)
{
    std::pair<int, int> topLeftOffsets, botRightOffsets;

    if (!_dstDataset)
        return false;

    if (!dmExtent(topLeftIn, botRightIn).overlaps(getRasterExtent()))
    {
        imgOffsetX = 0;
        imgOffsetY = 0;
        imgWidth = 0;
        imgHeight = 0;

        return true;
    }

    topLeftOffsets = getRasterPixelOffsetAt(topLeftIn);
    botRightOffsets = getRasterPixelOffsetAt(botRightIn, false);

    imgOffsetX = topLeftOffsets.first;
    imgOffsetY = topLeftOffsets.second;
    imgWidth = botRightOffsets.first - imgOffsetX;
    imgHeight = botRightOffsets.second - imgOffsetY;

    topLeftOut.east = _geoTransform[0] + imgOffsetX * _geoTransform[1];
    topLeftOut.north = _geoTransform[3] + imgOffsetY * _geoTransform[5];
    botRightOut.east = topLeftOut.east + imgWidth * _geoTransform[1];
    botRightOut.north = topLeftOut.north + imgHeight * _geoTransform[5];

    return true;
}


std::vector<std::string> dmDataset::getGdaldemOptionsVec()
{
    std::vector<std::string> options;

    switch (_visScheme)
    {
    case HILLSHADE:
        options.push_back("-az");
        options.push_back(std::to_string(_hillshadeParamAzimuth));
        options.push_back("-alt");
        options.push_back(std::to_string(_hillshadeParamAltitude));
        if (_hillshadeParamMultidirectional)
            options.push_back("-multidirectional");
        break;
    case COLOR_RELIEF:
        options.push_back("-alpha");
        options.push_back("-nearest_color_entry");
        break;
    case NONE:
    default:
        break;
    }

    return options;
}

std::vector<std::string> dmDataset::getGdalwarpOptionsVec()
{
    std::vector<std::string> options;

    options.push_back("-t_srs");
    options.push_back(_dstWkt.c_str());
    options.push_back("-nosrcalpha");

    options.push_back("-r");
    switch (_visScheme)
    {
    case HILLSHADE:
    case COLOR_RELIEF:
        options.push_back("max");
        break;
    case NONE:
        options.push_back("cubic");
        break;
    }

    return options;
}


gdalUtilOpts dmDataset::optionsVecToGdalUtilOpts(std::vector<std::string> &options)
{
    char** optionsArr = new char*[options.size() + 1];
    for (unsigned int i = 0; i<options.size(); i++)
    {
        optionsArr[i] = (char*)options[i].c_str();
    }
    optionsArr[options.size()] = nullptr;
    return gdalUtilOpts (optionsArr);
}

bool dmDataset::latLonToDstSrs(coord latLonIn, coord &dstSrsOut)
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

    from = proj_coord(proj_torad(latLonIn.east), proj_torad(latLonIn.north), 0, 0);

    to = proj_trans(projection, PJ_FWD, from);

    dstSrsOut.north = to.enu.n;
    dstSrsOut.east  = to.enu.e;

    /* Clean up */
    proj_destroy(projection);

    return true;
}

bool dmDataset::latLonToDstSrs(dmExtent latLonIn, dmExtent &dstSrsOut)
{
    bool success = true;
    
    success &= latLonToDstSrs(latLonIn.topLeft, dstSrsOut.topLeft);
    success &= latLonToDstSrs(latLonIn.botRight, dstSrsOut.botRight);

    return success;
}

GDALDataset * dmDataset::reprojectDataset(GDALDataset *dsToReproject)
{
    if (dsToReproject)
    {
        int err = 0;
        GDALDataset *warpedDS;

        std::vector<std::string> optionsVec = getGdalwarpOptionsVec();
        gdalUtilOpts options =  optionsVecToGdalUtilOpts(optionsVec);

        // coordinate system reprojection
        GDALWarpAppOptions *psWarpOptions = GDALWarpAppOptionsNew(options.get(), NULL);
        warpedDS = (GDALDataset*)GDALWarp((_tempFolderPath + warpedFileName).c_str(), NULL, 1, (GDALDatasetH*)&dsToReproject, psWarpOptions, &err);

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

 
        std::vector<std::string> optionsVec = getGdaldemOptionsVec();
        gdalUtilOpts options =  optionsVecToGdalUtilOpts(optionsVec);

    GDALDEMProcessingOptions * gdaldemOptions = GDALDEMProcessingOptionsNew(options.get(), NULL);

    switch (_visScheme)
    {
    case HILLSHADE:
        resultDs = (GDALDataset*)GDALDEMProcessing((_tempFolderPath + visFileName).c_str(), dsToVisualize, "hillshade", NULL, gdaldemOptions, &err);
        break;

    case COLOR_RELIEF:
        resultDs = (GDALDataset*)GDALDEMProcessing((_tempFolderPath + visFileName).c_str(), dsToVisualize, "color-relief", _colorConfFilename.c_str(), gdaldemOptions, &err);
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
