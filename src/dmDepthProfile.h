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

#ifndef _DM_DEPTH_PROFILE_
#define _DM_DEPTH_PROFILE_

#include <iostream>
#include <utility>  // std::pair
#include <vector>
#include "dm_API.h" // coord
#include "dm_types.h"

class  dmDataset;
class  dmDrawingState;

class dmDepthProfile
{
public:

    dmDepthProfile(dmDataset& ds, const dmRoute& route);
    ~dmDepthProfile();

private:

    dmDepthProfileData getLegProfile(dmDataset& dataset, dmLeg legInWm);
    dmDepthProfileData getRouteProfile(dmDataset& dataset, dmRoute route);

    dmDepthProfileData profileData;

    std::pair<double, double> getGridSize(dmDataset &dataset);
    std::pair<double, double> getNyquistSteps(std::pair<double, double> gridSize);

    friend std::ostream& operator<<(std::ostream& os, const dmDepthProfile& dp);

};

std::ostream& operator<<(std::ostream& os, const dmDepthProfile& dp);

#endif _DM_DEPTH_PROFILE_
