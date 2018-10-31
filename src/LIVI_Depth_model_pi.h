/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  LIVI Depth model Plugin
 * Author:   Marjukka Kokkonen, Sitowise Oy
 *
 ***************************************************************************
 *   Copyright (C) 2018 by Liikennevirasto & Sitowise                      *
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

#ifndef _LIVI_DEPTH_MODEL_PI_H_
#define _LIVI_DEPTH_MODEL_PI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

//#include <wx/fileconf.h>    // wxFileName

#include "dm_API.h" // DM_colourType

#include "ocpn_plugin.h" //Required for OCPN plugin functions
#include "LIVI_Depth_model_pi_UI_impl.h"
#include "dmDrawingState.h"

#include "version.h"

#define     MY_API_VERSION_MAJOR    1
 /* Minor version upto 15 at time of writing. Use the oldest one possible,
 to ensure compatibility to as many old users as possible. */
#define     MY_API_VERSION_MINOR    15

class Dlg;
class dmConfigHandler;
class dmColourfileHandler;
class dmDepthModelDrawer;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define LIVI_DEPTH_MODEL_TOOL_POSITION    -1          // Request default positioning of toolbar tool

/**
* Plugin for taking depth data into account for route planning
*/
class LIVI_Depth_model_pi : public opencpn_plugin_115 // upto 115
{
public:
    LIVI_Depth_model_pi(void *ppimgr);
    ~LIVI_Depth_model_pi(void);

    wxIcon GetIcon();

////  The required PlugIn Methods ////
    virtual int  Init(void)             override;
    virtual bool DeInit(void)           override;

    virtual int GetAPIVersionMajor()    override;    // impl. OK
    virtual int GetAPIVersionMinor()    override;    // impl. OK
    virtual int GetPlugInVersionMajor() override;    // impl. OK
    virtual int GetPlugInVersionMinor() override;    // impl. OK
    virtual wxBitmap *GetPlugInBitmap() override;    // impl. OK

    virtual wxString GetCommonName()        override;   // impl. OK
    virtual wxString GetShortDescription()  override;   // impl. OK
    virtual wxString GetLongDescription()   override;   // impl. OK

////  The optional overridable PlugIn Methods ////

    virtual void SetDefaults(void)          override;
//  int GetToolbarToolCount(void)           override;  // Used nowhere by OpenCPN, so not reqired?
    virtual void ShowPreferencesDialog(wxWindow* parent); // Preferences dialog not implemented (yet?)
//  virtual bool RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp) override; // OpenGL overlay function, OpenGL not implemented (yet?)
    virtual void SetCursorLatLon        (double lat, double lon) override;
    virtual void SetCurrentViewPort     (PlugIn_ViewPort &vp)   override;
    virtual void ProcessParentResize    (int x, int y)          override;
    virtual void SetColorScheme         (PI_ColorScheme cs)     override;
    virtual void OnToolbarToolCallback      (int id)            override;
    virtual void OnContextMenuItemCallback  (int id)            override;
    virtual wxArrayString GetDynamicChartClassNameArray(void)   override;
    virtual bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)   override; // opencpn_plugin_17
    virtual void SetPluginMessage(wxString &message_id,
        wxString &message_body)                     override;   // opencpn_plugin_17
    virtual void OnSetupOptions(void)               override;   // opencpn_plugin_19
    virtual void LateInit(void)                     override;   // opencpn_plugin_110
    virtual bool MouseEventHook(wxMouseEvent &event) override; // opencpn_plugin_112
//  virtual bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);

//  Other public methods
    void OnDepthModelDialogClose();

// LIVI additions
    // Additional functions for About dialog
    virtual wxString GetLongPluginVersionString();
    virtual wxString GetCopyright();

    // reactions to UI changes
    void OnImageFileChange(wxFileName fname);
    void OnGenerateImage(wxFileName fullFileName);

    void OnChartTypeChange      (int selectionId);
    void OnColourSchemaChange   (int selectionId);

    void OnColorOptionsApply();
    void OnUserColourFileChange(wxFileName fullFileName);

private:

    void OnClose( wxCloseEvent& event );

    LIVI_Depth_model_pi *plugin;
    int                 pluginToolId;

    wxWindow            *m_parent_window;

    dmConfigHandler     *m_pconf;
    Dlg                 *dialog;
    dmColourfileHandler *colourfileHandler;
    /** Icon bitmap of this plugin as icon.Needed by about dialog. */
    wxIcon              *m_icon;

    dmDepthModelDrawer  *dmDrawer;
  //wxFileName          pluginConfigPath;
    dmDrawingState      drawingState;

    bool                LoadConfig(void);
  //bool                SaveConfig(void);
    void                PushConfigToUI(void);
    void                PullConfigFromUI(void);
    DM_visualization    to_dmVisualizationType(int chartTypeId);
    DM_colourType       to_dmColourType(int colouringChoiceId);

    bool                m_ShowHelp,m_bCaptureCursor,m_bCaptureShip;
  //double              m_cursor_lon, m_cursor_lat;

};

#endif _LIVI_DEPTH_MODEL_PI_H_
