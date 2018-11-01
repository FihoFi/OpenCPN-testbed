#pragma once

#ifndef _DM_EXTENT_H_
#define _DM_EXTENT_H_

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
            this->east = other.east;
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

class dmExtent
{
public:
    dmExtent();
    dmExtent(coord topLeft, coord botRight);
    dmExtent(double top, double left, double bot, double right);

    dmExtent& operator= (const dmExtent& other);
    bool      operator==(const dmExtent& other);

    /**
    * @return   true, if area spanned by <i>other</i> is fully inside
    *           of this object's spanning area, or if the areas are equal.
    *           false else.
    */
    bool const isWithin(const dmExtent& other);

    /**
    * @return   true, if the area spanned by <i>other</i> overlaps with the
    *           area spanned by this object's, or if the areas are equal.
    *           false if there is no common point.
    */
    bool const overlaps(const dmExtent& other);

    /**
    * @todo How should we react, if the extents do not overlap?
    *
    * @return   The extent spanning the area common to <i>this</i> object,
    *           and the <i>other</i>.
    */
    dmExtent const getSectionWith(const dmExtent& other);

    /**
    * @todo How should we react, if the extents do not overlap?
    *
    * @return   The coordinates of <i>this</i> object, and the <i>other</i>,
    *           that span the maximal area.
    */
    dmExtent const getMaxes(const dmExtent& other);

private:
    coord topLeft;
    coord botRight;
};

#endif _DM_EXTENT_H_
