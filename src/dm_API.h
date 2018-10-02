#pragma once

#ifndef _DM_API_
#define _DM_API_


struct coord {
    coord()
        : lat(0), lon(0)
    {    }

    coord(double lat, double lon)
    {
        this->lat = lat;
        this->lon = lon;
    }
    /*float*/double lon; // mathematical, not geodetic x
    /*float*/double lat; // mathematical, not geodetic y
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
    * Returns the whole raster data in the Dataset.
    * The coordinate span of the dataset is returned in the parameters.
    * The ownership of the returned pointer is handed to the caller.
    *
    * @param[out] topLeftOut
    * @param[out] botRightOut
    */
    virtual unsigned char * getRasterData(
        coord &topLeftOut, coord &botRightOut) const = 0;

    /**
    * Returns a square part of the Dataset spanned by the two given (In)
    * coordinate pairs.
    * The coordinate span of the returned Dataset part  is returned
    * in the (Out) parameters.
    * The ownership of the returned pointer is handed to the caller.
    *
    * @param[in] topLeftIn  
    * @param[in] botRightIn
    * @param[out] topLeftOut
    * @param[out] botRightOut
    */
    virtual unsigned char * getRasterData(
        const coord topLeftIn, const coord botRightIn,
        coord &topLeftOut, coord &botRightOut) const = 0;
};
#endif _DM_API_