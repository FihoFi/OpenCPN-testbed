#pragma once

#include <string>

#ifndef _DM_API_
#define _DM_API_

struct coord;
struct dmExtent;

typedef enum DM_colourType
{
    COLOUR_UNDEFINED = 14703,  // Magic number much greater than 0, to avoid unintentionally accepted zeroes, and ones. Also More than wxID_HIGHEST = 5999, just in case the wxIDs could be used.

    COLOUR_USER_FILE,
    COLOUR_FIVE_RANGES,
    COLOUR_SLIDING,
    COLOUR_TWO_RANGES,

    COLOUR_MAX     // Not-to-be-used-in-code guardian value of the end of the enum range
} DM_colourType;
bool        dmColourTypeIsOk(DM_colourType col);
std::string colouringTypeToString(DM_colourType colType);

struct dmRasterImgData
{
    dmRasterImgData() :
        rgb(nullptr),
        alpha(nullptr)
    { }

    ~dmRasterImgData()
    {
        if (rgb)
        {
            delete rgb;
            rgb = nullptr;
        }
        if (alpha)
        {
            delete alpha;
            alpha = nullptr;
        }
    }

    unsigned char *rgb;
    unsigned char *alpha;
};

enum DM_visualization
{
    VISUALIZATION_UNDEFINED = 13579,  // Magic number much greater than 0, to avoid unintentionally accepted zeroes, and ones. Also More than wxID_HIGHEST = 5999, just in case the wxIDs could be used.

    NONE,
    HILLSHADE,
    COLOR_RELIEF,

    VISUALIZATION_MAX     // Not-to-be-used-in-code guardian value of the end of the enum range
};
bool        dmVisualizationIsOk(DM_visualization viz);
std::string chartTypeToString(DM_visualization chartType);

class /*DECL_EXP*/ dmLogWriter
{
public:
    //inline dmLogWriter();
    /** Destructor making this class abstract. */
    virtual ~dmLogWriter() = 0;

    virtual void logFatalError(const std::string message) = 0;
    virtual void logError     (const std::string message) = 0;
    virtual void logWarning   (const std::string message) = 0;
    virtual void logMessage   (const std::string message) = 0;
    virtual void logInfo      (const std::string message) = 0;
};

class /*DECL_EXP*/ dm_API
{
public:
    dm_API(dmLogWriter* logWriter);

    /** Destructor making this class abstract. */
    virtual ~dm_API() = 0;

    /**
    * Opens the dataset, defined by file <i>filename</i>,and transforms it to
    * World Mercator coordinate system.
    *
    * @return true, if opening, and transforming the dataset was successful, false else.
    */
    virtual bool openDataSet(const char* filename) = 0;
    //MRJ: is the same as bool setFileName(const char* filename);

    /**
    * Visualizes the dataset, opened in openDataSet.
    *
    * @return true, if visualization had no problems, false else.
    */
    virtual bool visualizeDataSet() = 0;

    /**
    * Returns width and height of the dataset in pixels.
    *
    * @param[out] width Width of the dataset in pixels
    * @param[out] height Height of the dataset in pixels
    *
    * @return true, if the dimensions were retrieved successfully, false otherwise
    */
    virtual bool getDatasetPixelDimensions(int &width, int &height) = 0;

    /**
    * Returns min and max (depth) values of the (reprojected) dataset.
    *
    * @param[out] min Minimum of the depths of the (reprojected) dataset
    * @param[out] max Maximum of the depths of the (reprojected) dataset
    *
    * @return true, if the extremes were retrieved successfully, false otherwise
    */
    virtual bool getDatasetExtremeValues(double& min, double& max) = 0;

    /**
    * Returns the extents of the World Mercator projected dataset as spanning
    * top-left, and bottom-right corners, in World Mercator coordinates.
    *
    * @param topLeft The top-left coordinate point of the WM projected chart
    * @param topLeft The bottom-right coordinate point of the WM projected chart

    * @return True, if the extents could be retrieved, false else.
    */
    virtual bool getDatasetExtents(coord &topLeft, coord &botRight) = 0;

    /**
    * Passes a full file path&name for a file where the colour configuration
    * information for a depth model chart can be found.
    *
    * @param[in] filename   Filepath of a file containing the colour configuration.
    * @param[in] giveOwnership  Tells, if the pointer deletion responsibility
    *                           is handled to the respondent.
    *
    * @return true, if the file exists (TODO ?and is a valid colour configuration file? TODO)
    */
    virtual bool setColourConfigurationFile(const char* filename, bool giveOwnership) = 0;

    /**
    * Passes the colour configuration information directly in the
    * for a depth model chart can be found.
    *
    * @param[in] fileContents   Char array describing the colouring for any
    *                           generated depth model chart.
    * @param[in] giveOwnership  Tells, if the pointer deletion responsibility
    *                           is handled to the respondent.
    *
    * @return true, if (TODO ?the fileContents is a valid colour configuration? 
    *                        or ?the pointer is not null?
    */
    virtual bool setColourConfiguration(const char* fileContents, bool giveOwnership) = 0;

    /**
    * Sets the visualization scheme that will be used when opening and converting a
    * raster data file to a drawable format. openDataset() should be called after this
    * for the new setting to take effect.
    *
    * @param[in] visScheme the desired visualization scheme
    * @return true, if given enumeration was ok, false else
    */
    virtual bool setVisualizationScheme(DM_visualization visScheme) = 0;

    /**
    * Returns the whole raster data in the Dataset in World Mercator
    * coordinate system, unscaled.
    * The coordinate span of the returned dataset, is returned in the
    * xxXxOut parameters, in World Mercator coordinates.
    * The ownership of the returned pointer stays in the implementing class.
    *
    * @param[out] topLeftOut  The top-left coordinate corner of the returned
    *                       image, in World Mercator coordinates.
    * @param[out] botRightOut The bottom-right coordinate corner of the returned
    *                       image, in World Mercator coordinates.
    *
    * @return Pointer to the whole, unscaled raster image data in World Mercator
    *         coordinate system
    */
    virtual dmRasterImgData * getRasterData(
        coord &topLeftOut, coord &botRightOut) = 0;

    /**
    * Returns a (possibly) cropped square part of the Dataset in World Mercator
    * coordinate system, unscaled, spanned by the two given xxXxIn WM coordinate pairs.
    * The coordinate span of the returned dataset part is returned
    * in the xxXxOut parameters, in World Mercator coordinates.
    * The ownership of the returned pointer stays in the implementing class.
    * The xxXxOut coordinates are set to nearest whole pixels of the whole
    * dataset, that are outside the cropping area, defined by the xxXxIn coordinates
    *
    * @param[in] topLeftIn  The top-left coordinate corner of the wanted cropped
    *                       image, in World Mercator coordinates.
    * @param[in] botRightIn The bottom-right coordinate corner of the wanted cropped
    *                       image, in World Mercator coordinates.
    * @param[out] topLeftOut  The top-left coordinate corner of the returned cropped
    *                       image, in World Mercator coordinates.
    * @param[out] botRightOut The bottom-right coordinate corner of the returned cropped
    *                       image, in World Mercator coordinates.
    * @param[out] imgWidth  The width  of the resulting image, in pixels.
    * @param[out] imgHeight The height of the resulting image, in pixels.
    *
    * @return Pointer to the cropped, unscaled raster image data in World Mercator
    *         coordinate system
    */
    virtual dmRasterImgData * getRasterData(
        const coord topLeftIn, const coord botRightIn,
        coord &topLeftOut, coord &botRightOut,
        int &imgWidth, int &imgHeight) = 0;

    /**
    * Transforms input coordinates (given in the SRS of the current destination
    * dataset) into latitude and longitude values. 
    *
    * @param[in] dstSRsIn input coordinates in destination dataset SRS
    * @param[out] latLonOut transformed coordinates as latitude and longitude
    */
    virtual bool dstSrsToLatLon(coord dstSrsIn, coord &latLonOut) = 0;
    virtual bool dstSrsToLatLon(dmExtent dstSrsIn, dmExtent &latLonOut) = 0;

    /**
    * Transforms input coordinates (given as latitude and longitude) into
    * the SRS of the current destination dataset.
    *
    * @param[in] latLonIn input coordinates as latitude and longitude
    * @param[out] dstSrsOut transformed coordinates in destination dataset SRS
    */
    virtual bool latLonToDstSrs(coord latLonIn, coord &dstSrsOut) = 0;
    virtual bool latLonToDstSrs(dmExtent latLonIn, dmExtent &dstSrsOut) = 0;


    /* Setters for hillshade parameters */

    /**
    * Sets zFactor parameter for hillshade visualization type.
    *
    * @param[in] zFactor vertical exaggeration used to pre-multiply the
    *                    elevations [GDAL documentation]
    */
    virtual void setHillshadeZFactor(double zFactor) = 0;

    /**
    * Sets scale parameter for hillshade visualization type.
    *
    * @param[in] scale ratio of vertical units to horizontal. If the horizontal
    *                  unit of the source DEM is degrees (e.g Lat/Long WGS84
    *                  projection), you can use scale=111120 if the vertical units
    *                  are meters (or scale=370400 if they are in feet) [GDAL documentation]
    */
    virtual void setHillshadeScale(double scale) = 0;


    /**
    * Sets azimuth parameter for hillshade visualization type. Default value on
    * initialization is 315.
    *
    * @param[in] azimuth azimuth of the light, in degrees. 0 if it comes from
    *                    the top of the raster, 90 from the east, ... The
    *                    default value, 315, should rarely be changed as it is
    *                    the value generally used to generate shaded maps. [GDAL documentation]
    */
    virtual void setHillshadeAzimuth(double azimuth) = 0;


    /**
    * Sets altitude parameter for hillshade visualization type. Default value on
    * initialization is 45.
    *
    * @param[in] altitude altitude of the light, in degrees. 90 if the light
    *                     comes from above the DEM, 0 if it is raking light. [GDAL documentation]
    */
    virtual void setHillshadeAltitude(double altitude) = 0;


    /**
    * Sets combined flag for hillshade visualization type.
    *
    * @param[in] combined combined shading, a combination of slope and oblique
                          shading [GDAL documentation]
    */
    virtual void setHillshadeCombined(bool combined) = 0;


    /**
    * Sets multidirectional flag for hillshade visualization type. This flag is
    * false by default.
    *
    * @param[in] multidirectional multidirectional shading, a combination of
    *                             hillshading illuminated from 225 deg, 270 deg,
    *                             315 deg, and 360 deg azimuth. [GDAL documentation]
    */
    virtual void setHillshadeMultidirectional(bool multidirectional) = 0;

    /**
    * Sets alpha value for hillshade visualization type. Default value on
    * initialization is 128.
    *
    * @param[in] alpha overall alpha value for hillshade visualization type
    */
    virtual void setHillshadeAlpha(unsigned char alpha) = 0;

    dmLogWriter* logWriter;
};

#endif _DM_API_
