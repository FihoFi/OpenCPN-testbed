#pragma once

#ifndef _DM_API_
#define _DM_API_


struct coord {
    coord()
        : north(0), east(0)
    {    }

    coord(double north, double east)
    {
        this->north = north;
        this->east = east;
    }

    bool operator=(const coord& other)
    {
        if (this != &other)
        {
            this->north = other.north;
            this->east  = other.east;
        }
        return this;
    }

    bool operator==(const coord& other)
    {
        if (this->north == other.north && this->east == other.east)
            return true;
        else
            return false;
    }

    /*float*/double east; // mathematical x, longitude etc.
    /*float*/double north; // mathematical y, latitude
};

struct dmRasterImgData
{
    dmRasterImgData() :
        rgb(nullptr),
        alpha(nullptr)
    { }

    ~dmRasterImgData()
    {
        delete rgb;
        delete alpha;
    }

    unsigned char *rgb;
    unsigned char *alpha;
};

enum DM_visualization
{
    NONE = 13579,  // Magic number much greater than 0, to avoid unintentionally accepted zeroes, and ones. Also More than wxID_HIGHEST = 5999, just in case the wxIDs could be used.
    HILLSHADE,
    COLOR_RELIEF
};


class /*DECL_EXP*/ dmLogWriter
{
public:
    //inline dmLogWriter();
    /** Destructor making this class abstract. */
    virtual ~dmLogWriter() = 0;

    virtual void logFatalError(const char* message) = 0;
    virtual void logError(const char* message) = 0;
    virtual void logWarning(const char* message) = 0;
    virtual void logMessage(const char* message) = 0;
    virtual void logInfo(const char* message) = 0;
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
    * Returns width and height of the dataset in pixels.
    *
    * @param[out] width Width of the dataset in pixels
    * @param[out] height Height of the dataset in pixels
    *
    * @return true, if the dimensions were retrieved successfully, false otherwise
    */
    virtual bool getDatasetPixelDimensions(int &width, int &height) = 0;

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
    * The ownership of the returned pointer is handed to the caller.
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
    * The ownership of the returned pointer is handed to the caller.
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


    /* Setters for hillshade parameters */

    /**
    * Sets zFactor parameter for hillshade visualization type.
    *
    * @param[in] zFactor vertical exaggeration used to pre-multiply the
    *                    elevations [GDAL documentation]
    *
    * @return false if an error occurred, true otherwise
    */
    virtual bool setHillshadeZFactor(double zFactor) = 0;

    /**
    * Sets scale parameter for hillshade visualization type.
    *
    * @param[in] scale ratio of vertical units to horizontal. If the horizontal
    *                  unit of the source DEM is degrees (e.g Lat/Long WGS84
    *                  projection), you can use scale=111120 if the vertical units
    *                  are meters (or scale=370400 if they are in feet) [GDAL documentation]
    *
    * @return false if an error occurred, true otherwise
    */
    virtual bool setHillshadeScale(double scale) = 0;


    /**
    * Sets azimuth parameter for hillshade visualization type. Default value on
    * initialization is 315.
    *
    * @param[in] azimuth azimuth of the light, in degrees. 0 if it comes from
    *                    the top of the raster, 90 from the east, ... The
    *                    default value, 315, should rarely be changed as it is
    *                    the value generally used to generate shaded maps. [GDAL documentation]
    *
    * @return false if an error occurred, true otherwise
    */
    virtual bool setHillshadeAzimuth(double azimuth) = 0;


    /**
    * Sets altitude parameter for hillshade visualization type. Default value on
    * initialization is 45.
    *
    * @param[in] altitude altitude of the light, in degrees. 90 if the light
    *                     comes from above the DEM, 0 if it is raking light. [GDAL documentation]
    *
    * @return false if an error occurred, true otherwise
    */
    virtual bool setHillshadeAltitude(double altitude) = 0;


    /**
    * Sets combined flag for hillshade visualization type.
    *
    * @param[in] combined combined shading, a combination of slope and oblique
                          shading [GDAL documentation]
    *
    * @return false if an error occurred, true otherwise
    */
    virtual bool setHillshadeCombined(bool combined) = 0;


    /**
    * Sets multidirectional flag for hillshade visualization type. This flag is
    * false by default.
    *
    * @param[in] multidirectional multidirectional shading, a combination of
    *                             hillshading illuminated from 225 deg, 270 deg,
    *                             315 deg, and 360 deg azimuth. [GDAL documentation]
    *
    * @return false if an error occurred, true otherwise
    */
    virtual bool setHillshadeMultidirectional(bool multidirectional) = 0;

    dmLogWriter* logWriter;
};

#endif _DM_API_
