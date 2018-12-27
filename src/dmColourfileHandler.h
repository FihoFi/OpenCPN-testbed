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

#ifndef _DM_COLOURFILE_HANDLER_H_
#define _DM_COLOURFILE_HANDLER_H_

#include <wx/fileconf.h>    // wxFileName

class dmConfigHandler;
enum DM_colourType;

class dmColourfileHandler
{
public:
    dmColourfileHandler(dmConfigHandler* confH, wxString tempDataDirectoryPath);

    dmColourfileHandler& operator= (const dmColourfileHandler& other);

    void            setChartExtremeValues(double min, double max);
    bool            GetConfFileOfType(DM_colourType colourOption, wxFileName& colorFile);

private:
    bool            GenerateConfFileOfType(DM_colourType colourOption);
    bool            GenerateUserColorConfFile();
    bool            GenerateFiveColorConfFile();
    bool            GenerateSlidingColorConfFile();
    bool            GenerateTwoColorConfFile();

    bool            GenerateColorConfFile(wxFileName &confPath,
                        const wxString fileName, const wxString confText);

    wxString        GetUserColourDepthColourWks();
    wxString        AppendWaterLevelsToConfLine(wxString line, int lineNr);
    wxString        GetFiveColourDepthColourWks();
    wxString        GetSlidingColourDepthColourWks();
    wxString        GetTwoColourDepthColourWks();

    wxFileName      userColoursFileName;
    wxFileName      fiveColoursFileName;
    wxFileName      slidingColoursFileName;
    wxFileName      twoColoursFileName;

    dmConfigHandler* m_pconf;
    wxString        tempDataDirectoryPath;
    double          chartMin, chartMax;
};

#endif _DM_COLOURFILE_HANDLER_H_
