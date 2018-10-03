#include <algorithm> 
#include <cctype>
#include <locale>
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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "LIVI_Depth_model_pi.h"
#include "LIVI_Depth_model_pi_UI_impl.h"
#include "LIVI_Depth_model_pi_UI.h"

#include "dmConfigHandler.h"    // For handling config options
#include "dmDepthModelDrawer.h"

class LIVI_Depth_model_pi;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new LIVI_Depth_model_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    LIVI_Depth_model PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

LIVI_Depth_model_pi::LIVI_Depth_model_pi(void *ppimgr)
      :opencpn_plugin_115 (ppimgr)
    , m_parent_window(NULL)
    , m_pconf(NULL)
    , dmDrawer(NULL)
{
	  m_bShowLIVI_Depth_model = false;
    // Create the PlugIn icons
    initialize_images();
}

LIVI_Depth_model_pi::~LIVI_Depth_model_pi(void)
{
     delete _img_LIVI_Depth_model_pi;
     delete _img_LIVI_Depth_model;
     
}

wxIcon LIVI_Depth_model_pi::GetIcon()
{
    return *m_icon;
}

int LIVI_Depth_model_pi::Init(void)
{
    AddLocaleCatalog( wxT("LIVI_Depth_model_pi") );

    m_icon = new wxIcon();
    m_icon->CopyFromBitmap(*_img_LIVI_Depth_model);
    dmDrawer = new dmDepthModelDrawer();

    // Set some default private member parameters
    m_dialog_x = 0;
    m_dialog_y = 0;
    ::wxDisplaySize(&m_display_width, &m_display_height);

    //    Get a pointer to the opencpn display canvas, ...
    // ... to use as a parent for the POI Manager dialog
    m_parent_window = GetOCPNCanvasWindow();

    m_pconfig = GetOCPNConfigObject(); // configuration file
    LoadConfig();

    //    This PlugIn needs a toolbar icon, so request its insertion
	if(m_bLIVI_Depth_modelShowIcon)
        m_leftclick_tool_id  = InsertPlugInTool(wxT(""),
            _img_LIVI_Depth_model, _img_LIVI_Depth_model, wxITEM_CHECK,
            _("LIVI_Depth_model"), wxT(""), NULL,
            LIVI_DEPTH_MODEL_TOOL_POSITION, 0, this);

    m_pDialog = NULL;

    m_chartBase = new PlugInChartBase();

    return (
        WANTS_OVERLAY_CALLBACK | // pluginManager calls this->RenderOverlay(wxDC ...).
//TODO  WANTS_CURSOR_LATLON    | // pM calls this->SetCursorLatLon. Implement when depths can be drawn, and want to get depth in single point?
        WANTS_TOOLBAR_CALLBACK | // pM/OpenCPN DOES NOT REACT. Returned by all the plugins I know..? Any plugin has at least a single icon in the toolbar, that wants to react to a click?
        INSTALLS_TOOLBAR_TOOL  | // pM/OpenCPN DOES NOT REACT. Returned by all the plugins I know..? Any plugin has at least a single icon in the toolbar?
        WANTS_CONFIG           | // pM/OpenCPN DOES NOT REACT. This plugin uses config file just used as in example DR_pi, getting gonf file by GetOCPNConfigObject.
        INSTALLS_TOOLBOX_PAGE  | // pM calls OnSetupOptions & OnCloseToolboxPanel
        INSTALLS_CONTEXTMENU_ITEMS     | // pM/OpenCPN DOES NOT REACT.
//      WANTS_NMEA_SENTENCES           | // pM calls SetNMEASentence. Not needed. For interpreting nautical communication.
//      WANTS_NMEA_EVENTS              | // pM calls SetPositionFix.  Not needed. For interpreting nautical communication.
//      WANTS_AIS_SENTENCES            | // pM calls SetAISSentence.  If ship's location information required.
//      USES_AUI_MANAGER               | // pM calls UpdateAuiStatus. If handling panes, docking, etc. is required. wxAuiManager(:wxEvtHandler) got by GetFrameAuiManager. 
        WANTS_PREFERENCES              | // pM calls ShowPreferencesDialog. Implement if need to set some Plugin preferences from the Options/Plugins page.
//      INSTALLS_PLUGIN_CHART          | // pM calls ChartData->UpdateChartClassDescriptorArray and GetDynamicChartClassNameArray. Maybe? what is the plugin chart for..? Difference/similarity to OVERLAY_CALLBACK?
        WANTS_ONPAINT_VIEWPORT         | // pM calls SetCurrentViewPort.
        WANTS_PLUGIN_MESSAGING         | // pM calls SetPluginMessage.
// OGL  WANTS_OPENGL_OVERLAY_CALLBACK  | // pM calls RenderGLOverlay(wxGLContext ...). OpenGL is extra. Do not implement yet, if requires any effort.
// OGL  WANTS_DYNAMIC_OPENGL_OVERLAY_CALLBACK | //pM>RenderGLOverlay(wxGLContext ...)., chart1 forces full canvas refresh at ~1hz
        WANTS_LATE_INIT                 // pM calls LateInit. Implement e.g. for loading charts, to not to hang the main program.
// OGL  INSTALLS_PLUGIN_CHART_GL       | // pM calls GetDynamicChartClassNameArray.    OpenGL is extra. Do not implement yet, if requires any effort.
//      WANTS_MOUSE_EVENTS             | // pM calls MouseEventHook.            Implement if need to listen to mouse clicks on chart, e.g. depth information on clicks
//      WANTS_VECTOR_CHART_OBJECT_INFO | // pM calls SendVectorChartObjectInfo. Initiated by cm93, or s57chart objects.
//      WANTS_KEYBOARD_EVENTS          | // pM calls KeyboardEventHook.         Implement if need to listen to keyboard input.
    );
}

bool LIVI_Depth_model_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pDialog)
      {
            //Capture dialog position
            wxPoint p = m_pDialog->GetPosition();
            SetLIVIDepthModelDialogX(p.x);
            SetLIVIDepthModelDialogY(p.y);
            m_pDialog->Close();
            delete m_pDialog;
            m_pDialog = NULL;

			m_bShowLIVI_Depth_model = false;
			SetToolbarItemState( m_leftclick_tool_id, m_bShowLIVI_Depth_model );
      }	
    
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh mainn window
    delete dmDrawer;

    return true;
}

int LIVI_Depth_model_pi::GetAPIVersionMajor()    {  return MY_API_VERSION_MAJOR;     }
int LIVI_Depth_model_pi::GetAPIVersionMinor()    {  return MY_API_VERSION_MINOR;     }

int LIVI_Depth_model_pi::GetPlugInVersionMajor() {  return PLUGIN_VERSION_MAJOR;     }
int LIVI_Depth_model_pi::GetPlugInVersionMinor() {  return PLUGIN_VERSION_MINOR;     }

wxBitmap *LIVI_Depth_model_pi::GetPlugInBitmap() {  return _img_LIVI_Depth_model_pi; }

wxString LIVI_Depth_model_pi::GetCommonName()    {  return _("LIVI Depth model");    }

wxString LIVI_Depth_model_pi::GetShortDescription()
{
      return _( "Allows Depth model data to be taken into account\n"
                "when planning routes");
}

wxString LIVI_Depth_model_pi::GetLongDescription()
{
      return _( "Allows Depth model data to be taken into account\n"
                "when planning routes.\n"
                "Project administered by LIVI (Liikennevirasto -"
                "Finnish transport agency)");
}

/**
* @TODO NOT IMPLEMENTED yet.
* This will be called upon enabling a PlugIn via the user Dialog
* Chance to setup any default options and behavior.
* Set any variable loadable from (possible Preferences window, and)
* config file(s).
*/
void LIVI_Depth_model_pi::SetDefaults(void)
{
    // TODO
}

//int LIVI_Depth_model_pi::GetToolbarToolCount(void)
//{    return 1;        /* 1 - The options dialog, 2 -The draw-a-dot test */}

/**
* @TODO NOT IMPLEMENTED yet.
* If this plugin is set to be WANTS_PREFERENCES-able, the
* Preferences button is enabled on the Options/Plugins page.
* Generate, and handle that dialog here.
*/
void LIVI_Depth_model_pi::ShowPreferencesDialog(wxWindow* parent)
{
    // TODO
}

/** OpenGL overlay function, OpenGL not implemented (yet) */
//bool RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp);

/**
* @TODO NOT IMPLEMENTED yet.
* Usable, if WANTS_CURSOR_LATLON-able.
* Implement this when depths can be drawn, and want e.g. to pass
* the user the depth value in a single point at cursor.
*/
void LIVI_Depth_model_pi::SetCursorLatLon(double lat, double lon)
{
    //TODO
}

/**
* @TODO NOT IMPLEMENTED yet.
* Passes the current viewport of the main program th this plugin.
* Save it to be able to use it, e.g. to refresh/ load again the maps
* that should be currently visible, or something.
* NOTE that RenderOverlay also passes the viewPort. So, this might not be
* required?
*/
void LIVI_Depth_model_pi::SetCurrentViewPort(PlugIn_ViewPort &vp)
{
    coord topLeft (vp.lat_max, vp.lon_min);
    coord botRight(vp.lat_min, vp.lon_max);
 
    bool success = dmDrawer->applyChartAreaData(topLeft, botRight);
}

/**
* @TODO NOT IMPLEMENTED yet.
* React to OCPN window resize. The other plugins I have watched onto,
* do not use this, so you may probably go well without.
*/
void LIVI_Depth_model_pi::ProcessParentResize(int x, int y)
{
    //TODO
}

/**
* Basic implementation as DimeWindow, by DR pi.
* Call if there is need to handle day/dusk/night coloring
* of the drawn items.
* Not very high priority for the Depth model at start at least,
* as the route planning will most probably be done at daylight.
* BUT if the depth model chart is too bright, and is required
* to be shown at dusk/night conditions too, they may need
* reprocessing of the colours here.
*/
void LIVI_Depth_model_pi::SetColorScheme(PI_ColorScheme cs)
{
    if (NULL == m_pDialog)
        return;

    DimeWindow(m_pDialog);
}

/**
* Basics implemented already by DR pi.
* This function shows/hides the UI window, and initiates it if non-existant.
*/
void LIVI_Depth_model_pi::OnToolbarToolCallback(int id)
{
    if (m_pDialog == NULL)
    {
        m_pDialog = new Dlg(m_parent_window);
        m_pDialog->plugin = this;
        m_pDialog->Move(wxPoint(m_dialog_x, m_dialog_y));
    }

    m_pDialog->Fit();

    // Toggle : update toggle state, and window show status
    m_bShowLIVI_Depth_model = !m_bShowLIVI_Depth_model;
    if(m_bShowLIVI_Depth_model) {   m_pDialog->Show();  }
    else                        {   m_pDialog->Hide();  }

    // Toggle is handled by the toolbar but we must keep plugin manager
    // b_toggle updated
    // to actual status to ensure correct status upon toolbar rebuild
    SetToolbarItemState(m_leftclick_tool_id, m_bShowLIVI_Depth_model);

    RequestRefresh(m_parent_window); // refresh main window
}

/**
* @TODO NOT IMPLEMENTED yet.
* Implement if want to react on context menu usage.
* @note ContextMenuItem is added by AddCanvasContextMenuItem
*/
void LIVI_Depth_model_pi::OnContextMenuItemCallback(int id)
{
    // TODO
}

/**
* @TODO NOT IMPLEMENTED yet.
* Called, if INSTALLS_PLUGIN_CHART-able
*/
wxArrayString LIVI_Depth_model_pi::GetDynamicChartClassNameArray(void)
{
    // TODO
    return wxArrayString();
}

/**
* RenderOverlay @since ocpn_plugin_17
* Called by pluginManager in RenderAllCanvasOverlayPlugIns.
* This is called, if this plugin has stated it WANTS_OVERLAY_CALLBACK
* in its capabilities return value.
*/
bool LIVI_Depth_model_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    bool success = dmDrawer->calculateDepthModelBitmap(*vp);
    success &= dmDrawer->drawDepthChart(dc/*, *vp*/);
    return success;

}

/**
* @since ocpn_plugin_17
* Implement this, if wanting to listen to messages from other
* plugins (e.g., if the BAG format reader is another plugin).
* This is called, if this plugin has stated it WANTS_PLUGIN_MESSAGING
* in its capabilities return value.
*/
void LIVI_Depth_model_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
/* // Example usage
//#include "wxJSON/jsonreader.h" // or where ever it can be found
//#include "wxJSON/jsonwriter.h" // or where ever it can be found
    if (message_id == _T("MY_QUERY_TO_BE_ANSWERED_ID")) {
        wxJSONValue v;
        v[wxT("BestColorEverKey")]  = "Orange";
        v[wxT("SecondBestColorKey")]= "Pink";
        wxJSONWriter w;
        wxString out;
        w.Write(v, out);
        SendPluginMessage(wxString(_T("BEST_COLOR")), out);
    }
    else if (message_id == _T("MY_MESSAGE_TO_BE_PARSED_ID")) {
        wxJSONValue  root;        // construct the JSON root object
        wxJSONReader reader;        // construct a JSON parser

        // read the JSON text to 'root' structure, check for errors
        int numErrors = reader.Parse(message_body, &root);
        if (numErrors > 0) {
            // const wxArrayString& errors = reader.GetErrors();
            return;
        }
        wxString decl = root[wxT("AFieldKey")].AsString();
    }
    */
}

/**
* @since ocpn_plugin_19
* Called by pluginManager in NotifySetupOptionsPlugin,
* This is called, if this plugin has stated itINSTALLS_TOOLBOX_PAGE
* in its capabilities return value.
*/
void LIVI_Depth_model_pi::OnSetupOptions(void)
{
    // TODO
}

/**
* @since ocpn_plugin_110
* Implement if e.g. want to load some maps at startup without
* slowing down OpenCPN startup.
* Called by pluginManager in CallLateInit, if this plugin has 
* stated it WANTS_LATE_INIT in its capabilities return value in Init().
*/
void LIVI_Depth_model_pi::LateInit(void)
{
    // TODO
}

wxString LIVI_Depth_model_pi::GetLongPluginVersionString() {
    std::string versionStr =
        PLUGIN_VERSION_MAJOR + '.' + PLUGIN_VERSION_MINOR +
        ',' + ' ' + PLUGIN_VERSION_DATE;
    return _("LIVI Depth model plugin, version "+ versionStr);
}

wxString LIVI_Depth_model_pi::GetCopyright() {
    return _("@ 2018, LIVI & Sitowise");
}





void LIVI_Depth_model_pi::PushConfigToUI(void)
{
    for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
        m_pDialog->SetCustomColor(i, m_conf.m_customColours[i]);
    }
    for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
        m_pDialog->SetCustomLevel(i, m_conf.m_customDepths[i]);
    }
}

void LIVI_Depth_model_pi::PullConfigFromUI(void)
{
    for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
    //    wxColour* col = m_pDialog->GetCustomColor(i);
        m_conf.m_customColours[i] = wxColour(m_pDialog->GetCustomColor(i));// *col;
    //    delete col;
    }
    for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
        m_conf.m_customDepths[i] = m_pDialog->GetCustomLevel(i);
    }
}
/* Loads config (ini) file entries of the Depth model. */
bool LIVI_Depth_model_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(pConf)
    {
        pConf->SetPath ( wxT( "/Settings/LIVI_Depth_model_pi" ) );
        pConf->Read ( wxT( "ShowLIVI_Depth_modelIcon" ), &m_bLIVI_Depth_modelShowIcon, 1 );

        m_dialog_x =  pConf->Read ( wxT ( "DialogPosX" ), 20L );
        m_dialog_y =  pConf->Read ( wxT ( "DialogPosY" ), 20L );

        if((m_dialog_x < 0) || (m_dialog_x > m_display_width))
            m_dialog_x = 5;
        if((m_dialog_y < 0) || (m_dialog_y > m_display_height))
            m_dialog_y = 5;

// LIVI additions
        for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
            std::string colour = pConf->Read(wxT("CustomColour") + std::to_string(i));
            if (sizeof(colour) == 0)
                colour = "#f0f0f0"; // grey as default, if no color found
                // #ff0000, #ffc4e4, #ffffff, #80c4ff, #0000ff

            unsigned char* valChar = (unsigned char*)colour.c_str();
            m_conf.m_customColours[i] = wxColour(valChar);
        }
      
        for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
            std::string str = pConf->Read(wxT("CustomDepth" + std::to_string(i)));
            if (str.length() == 0)
                str = std::to_string(DM_NUM_CUSTOM_DEP*5); // grey as default, if no color found
            m_conf.m_customDepths[i] = std::stoi(str);
        }

        pConf->Read(wxT("ConfigFilePath"), &m_config_file_full_path.GetFullPath());

// end of LIVI additions
        return true;
     }
     else
         return false;
}

/*  Saves data entries of the Depth model to the config settings.
    These are updated to the ini file at OpenCPN close. */
bool LIVI_Depth_model_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if (pConf)
    {
        pConf->SetPath(wxT("/Settings/LIVI_Depth_model_pi"));
        pConf->Write(wxT("ShowLIVI_Depth_modelIcon"), m_bLIVI_Depth_modelShowIcon);

        pConf->Write(wxT("DialogPosX"), m_dialog_x);
        pConf->Write(wxT("DialogPosY"), m_dialog_y);

// LIVI additions
        for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
            pConf->Write(wxT("CustomColour") + std::to_string(i),
                m_conf.m_customColours[i].GetAsString(wxC2S_HTML_SYNTAX));
        }
        for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
            pConf->Write(wxT("CustomDepth" + std::to_string(i)), m_conf.m_customDepths[i]);
        }

        pConf->Write(wxT("ConfigFilePath"), m_config_file_full_path.GetFullPath());

// end of LIVI additions
        return true;
     }
     else
         return false;
}

void LIVI_Depth_model_pi::OnLIVI_Depth_modelDialogClose()
{
    m_bShowLIVI_Depth_model = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowLIVI_Depth_model );
    m_pDialog->Hide();
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

}

// LIVI additions
void LIVI_Depth_model_pi::OnColorOptionsApply()
{
    PullConfigFromUI();
    SaveConfig();
}


/*
wxString &LIVI_Depth_model_pi::GetConfigFileName()
{
    if (m_LIVI_depth_model_config_file_name.IsEmpty()) {
        //      Establish the location of the config file
        wxFileName& std_path = GetStdPaths();

#ifdef __WXMSW__
        m_LIVI_depth_model_config_file_name = wxT("LIVI_depth_model.ini");
        m_LIVI_depth_model_config_file_name.Prepend(wxStandardPaths::GetHomeDir());

#elif defined __WXOSX__
        m_LIVI_depth_model_config_file_name = std_path.GetUserConfigDir(); // should be ~/Library/Preferences
        appendOSDirSlash(&m_LIVI_depth_model_config_file_name);
        m_LIVI_depth_model_config_file_name.Append(wxT("LIVI_depth_model"));
        appendOSDirSlash(&m_LIVI_depth_model_config_file_name);
        m_LIVI_depth_model_config_file_name.Append(wxT("LIVI_depth_model.ini"));
#else
        m_LIVI_depth_model_config_file_name = std_path.GetUserDataDir(); // should be ~/.opencpn
        appendOSDirSlash(&m_LIVI_depth_model_config_file_name);
        m_LIVI_depth_model_config_file_name.Append(wxT("LIVI_depth_model.conf"));
#endif

        if (g_bportable) {
            m_LIVI_depth_model_config_file_name = GetHomeDir();
#ifdef __WXMSW__
            m_LIVI_depth_model_config_file_name += wxT("LIVI_depth_model.ini");
#elif defined __WXOSX__
            m_LIVI_depth_model_config_file_name += wxT("LIVI_depth_model.ini");
#else
            m_LIVI_depth_model_config_file_name += wxT("LIVI_depth_model.conf");
#endif

        }

#ifdef __OCPN__ANDROID__
        m_LIVI_depth_model_config_file_name = androidGetPrivateDir();
        appendOSDirSlash(&m_LIVI_depth_model_config_file_name);
        m_LIVI_depth_model_config_file_name += wxT("opencpn.conf");
#endif

    }

    return m_LIVI_depth_model_config_file_name;
}
*/
/*
wxString &OCPNPlatform::GetPluginDir()
{
    if (m_PluginsDir.IsEmpty()) {

        wxStandardPaths& std_path = GetStdPaths();

        //  Get the PlugIns directory location
        m_PluginsDir = std_path.GetPluginsDir();   // linux:   {prefix}/lib/opencpn
                                                   // Mac:     appname.app/Contents/PlugIns
#ifdef __WXMSW__
        m_PluginsDir += wxT("\\plugins");             // Windows: {exe dir}/plugins
#endif

        if (g_bportable) {
            m_PluginsDir = GetHomeDir();
            m_PluginsDir += wxT("plugins");
        }

#ifdef __OCPN__ANDROID__
        // something like: data/data/org.opencpn.opencpn
        wxFileName fdir = wxFileName::DirName(std_path.GetUserConfigDir());
        fdir.RemoveLastDir();
        m_PluginsDir = fdir.GetPath();

#endif
    }

    return m_PluginsDir;
}
*/ // end of LIVI additions