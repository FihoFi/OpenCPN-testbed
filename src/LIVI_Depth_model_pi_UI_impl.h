/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  LIVI_Depth_model Plugin
 * Author:   Marjukka Kokkonen @ Sitowise
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Mike Rossiter                                   *
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

#ifndef _LIVI_DEPTH_MODEL_PI_UI_IMPL_H_
#define _LIVI_DEPTH_MODEL_PI_UI_IMPL_H_

#ifdef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "LIVI_Depth_model_pi_UI.h"
#include "LIVI_Depth_model_pi.h"
//#include "NavFunc.h"  // from original DR_pi plugin
#include "tinyxml.h"

#include <list>
#include <vector>

using namespace std;

class LIVI_Depth_model_pi;

class Position;

class Dlg : public LIVIDMUI_DLG
{
public:
        Dlg( wxWindow* parent, wxWindowID id = wxID_ANY,
            const wxString& title = _("LIVI Depth model Plugin"),
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize( -1,-1 ),
            long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

  //      void OnPSGPX( wxCommandEvent& event );		
		bool OpenXML();
		
//		vector<Position> my_positions;
//		vector<Position> my_points;

//        void Calculate( wxCommandEvent& event, bool Export, int Pattern );
//        void Addpoint(TiXmlElement* Route, wxString ptlat, wxString ptlon, wxString ptname, wxString ptsym, wxString pttype);

        wxFileName  GetColorConfigurationFileName();
        void        SetColorConfigurationFileName(wxFileName &fileName);

        void        SetCustomColor(int num, wxColour& col);
        wxColour    GetCustomColor(int num);
        void        SetCustomLevel(int num, int level);
        double      GetCustomLevel(int num);

        void        SetDepthChartFileName(wxFileName &fileName);
        wxFileName  GetDepthChartFileName();

        LIVI_Depth_model_pi *plugin; // for callbacks at UI events

//		wxString rte_start;
//	    wxString rte_end;

private:
	    void         OnClose                       ( wxCloseEvent&   WXUNUSED(event) );
        virtual void OnColorOptionsApplyButtonClick( wxCommandEvent& WXUNUSED(event) );
        virtual void OnAboutLIVIDepthModel         ( wxCommandEvent& WXUNUSED(event) );
        virtual void OnAboutWxWidgets              ( wxCommandEvent& WXUNUSED(event) );
        virtual void OnFileImportFileChange  ( wxFileDirPickerEvent& WXUNUSED(event) );

        bool dbg;

 //     double lat1, lon1, lat2, lon2;
 //     bool error_found;
 //		wxString     m_gpx_path;		
};


class Position
{
public:

    wxString lat, lon, wpt_num;
    Position *prev, *next; /* doubly linked circular list of positions */
    int routepoint;

};

#endif