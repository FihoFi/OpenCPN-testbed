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

#include <string>

#ifndef _DM_EXTENT_H_
#define _DM_EXTENT_H_

struct coord {
    coord();
    coord(double north, double east);

    bool operator=(const coord& other);
    bool operator==(const coord& other);

    std::string to_string() const;

    /*float*/double east; // mathematical x, longitude etc.
    /*float*/double north; // mathematical y, latitude
};

struct dmExtent
{
    coord topLeft;
    coord botRight;

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

    double const height();
    double const width();

    std::string to_string();
};

#endif _DM_EXTENT_H_
