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
    /*float*/double east; // mathematical x, longitude etc.
    /*float*/double north; // mathematical y, latitude
};

class /*DECL_EXP*/ dm_API
{
public:
    //inline dm_API();
    /** Destructor making this class abstract. */
    virtual ~dm_API() = 0;

    virtual bool openDataSet(const char* filename) = 0;
    //MRJ: is the same as bool setFileName(const char* filename);

    /**
    * Passes a full file path&name for a file where the colour configuration
    * information for a depth model chart can be found.
    *
    * @param[in] filename   Filepath of a file containing the colour configuration.
    * @param[in] giveOwnership  Tells, if the pointer deletion responsibility
    *                           is handled to the respondent.
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
    * @return true, if (TODO ?the fileContents is a valid colour configuration? 
    *                        or ?the pointer is not null?
    */
    virtual bool setColourConfiguration(const char* fileContents, bool giveOwnership) = 0;

     /**
    * Returns the whole raster data in the Dataset.
    * The coordinate span of the dataset is returned in the parameters.
    * The ownership of the returned pointer is handed to the caller.
    *
    * @param[out] topLeftOut 
    * @param[out] botRightOut
    */
    virtual unsigned char * getRasterData(
        coord &topLeftOut, coord &botRightOut) = 0;

    /**
    * Returns a square part of the Dataset spanned by the two given (In)
    * coordinate pairs.
    * The coordinate span of the returned Dataset part  is returned
    * in the (Out) parameters.
    * The ownership of the returned pointer is handed to the caller.
    *
    * @param[out] imgWidth desired width of the resulting image
    * @param[out] imgHeight desired height of the resulting image
    * @param[in] topLeftIn  
    * @param[in] botRightIn
    * @param[out] topLeftOut
    * @param[out] botRightOut
    */
    virtual unsigned char * getRasterData(int imgWidth, int imgHeight,
        const coord topLeftIn, const coord botRightIn,
        coord &topLeftOut, coord &botRightOut) = 0;
};
#endif _DM_API_