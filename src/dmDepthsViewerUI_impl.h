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

#ifndef _DM_SHOWDEPTHS_UI_IMPL_H_
#define _DM_SHOWDEPTHS_UI_IMPL_H_

#ifdef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "dmDepthsViewerUI.h"
#include "dm_API.h"

#include <list>
#include <vector>

//using namespace std;

class LIVI_Depth_model_pi;

class DepthsViewer: public dmDepthsViewerDialog      // Use one of these
//class DepthsViewer: public dmDepthsViewerPanel     // Use one of these
//class DepthsViewer: public dmDepthsViewerToolBar   // Use one of these
//class DepthsViewer: public dmDepthsViewerFrame     // Use one of these
{
public:
    static const double NotAvailableValue;

    DepthsViewer(wxWindow* parent, 
                 wxWindowID id = wxID_ANY,
                 const wxString& title = _("Depth model: depths viewer"),
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize /*wxSize(-1, -1)*/,
                 long style = wxTRANSPARENT_WINDOW | wxSTAY_ON_TOP
                            | wxTAB_TRAVERSAL
                            | wxRESIZE_BORDER | wxRESIZE_BORDER
                            | wxTB_HORIZONTAL);

    void    SetDepthInfoText(std::string infoStr);
    void    SetDepthErrorText(std::string errStr);
    void    SetCoordinatesText(double waterLevel = NotAvailableValue,
                               double cursorDepthInCD = NotAvailableValue,
                               double shipDepthInCD = NotAvailableValue);

//    LIVI_Depth_model_pi *plugin; // for callbacks at UI events
//
//private:
//    virtual void OnClose                       ( wxCloseEvent&   WXUNUSED(event) );
};

#endif _DM_SHOWDEPTHS_UI_IMPL_H_
