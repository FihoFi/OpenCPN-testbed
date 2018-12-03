/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  LIVI_Depth_model Plugin
 * Author:   Marjukka Kokkonen
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

#include "dmDepthsViewerUI_impl.h"
#include <wx/progdlg.h>
#include <wx/wx.h>
#include "wx/dir.h"
#include <list>
#include <cmath>
#include <iomanip> // setprecision
#include <sstream> // stringstream

#include "dm_API.h"

class dmDepthValues;

#define FAIL(X) do { error = X; goto failed; } while(0)

// magic number -9999(.000) chosen for compatibility with dmDataset depth return values
const double DepthsViewer::NotAvailableValue = -9999;

DepthsViewer::DepthsViewer(
      wxWindow* parent,         wxWindowID id,
      const wxString& title,    const wxPoint& pos,
      const wxSize& size,       long style)
    : dmDepthsViewerDialog  (parent, id, title, pos, size, style)   // Use one of these
//  : dmDepthsViewerPanel   (parent, id, pos, size, style)          // Use one of these
//  : dmDepthsViewerToolBar (parent, id, pos, size, style, title)   // Use one of these
//  : dmDepthsViewerFrame   (parent, id, title, pos, size, style)   // Use one of these
{
    this->Fit();
}

void DepthsViewer::SetDepthInfoText(std::string infoStr)
{
    this->dm_coordinates_staticText->SetLabel(infoStr);
    this->dm_coordinates_staticText->SetForegroundColour(*wxBLACK);
    this->dm_coordinates_staticText->Refresh();
}

void DepthsViewer::SetDepthErrorText(std::string errStr)
{
    this->dm_coordinates_staticText->SetLabel(errStr);
    this->dm_coordinates_staticText->SetForegroundColour(*wxRED);
    this->dm_coordinates_staticText->Refresh();
}

void DepthsViewer::SetCoordinatesText(  double waterLevel, double cursorDepthInCD,
                                        double shipDepthInCD)
{
    std::string signWL = waterLevel > 0 ? "+" : "";
    std::string signCursorCD = cursorDepthInCD > 0 ? "+" : "";
    std::string signShipCD = shipDepthInCD > 0 ? "+" : "";

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2);

    if (waterLevel > NotAvailableValue)
        stream << "\nCurrent water level: " << signWL << waterLevel;
    else
        stream << "\nCurrent water level: N/A";

    stream << "\nCursor:";
    if (cursorDepthInCD > NotAvailableValue) {
        stream  << "\n  Depth in chart datum: " << signCursorCD << cursorDepthInCD
                << "\n  Current depth in CD: " << (cursorDepthInCD + waterLevel);
    }
    else {
        stream  << "\n  Depth in chart datum: N/A"
                << "\n  Current depth in CD: N/A";
    }

    stream << "\nShip:";
    if (shipDepthInCD > NotAvailableValue) {
        stream  << "\n  Depth in chart datum: " << signShipCD << shipDepthInCD
                << "\n  Current depth in CD: " << (shipDepthInCD + waterLevel);
    }
    else {
        stream  << "\n  Depth in chart datum: N/A"
                << "\n  Current depth in CD: N/A";
    }

    std::string depthsStr = stream.str();
    this->dm_coordinates_staticText->SetLabel(depthsStr);
}

//void DepthsViewer::OnClose(wxCloseEvent& event)
//{
//    plugin->OnDepthsViewerClose();
//}
