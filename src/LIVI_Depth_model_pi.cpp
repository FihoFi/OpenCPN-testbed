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
#include "dmColourfileHandler.h" // For handling colour file access operations
#include "dmDepthModelDrawer.h"

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
    : opencpn_plugin_115 (ppimgr)
    , plugin(NULL)
    , m_parent_window(NULL)
    , m_pconf(NULL)
    , dialog(NULL)
    , colourfileHandler(NULL)
    , m_icon(NULL)
    , dmDrawer(NULL)
{
    // Create the PlugIn icons
    initialize_images();
    plugin = this;
}

LIVI_Depth_model_pi::~LIVI_Depth_model_pi(void)
{
    plugin = NULL; // To make sure we do not get a recursive destruction call
    deinitialize_images();
}

wxIcon LIVI_Depth_model_pi::GetIcon()
{
    return *m_icon;
}

/** Initiates anything that is to be kept alive anyway, whether
*  the plugin is "active" or not.
*  Thought to have here everything but picture data - the configuration
*  stuff does not take so much space. */
int LIVI_Depth_model_pi::Init(void)
{
    AddLocaleCatalog( _T("LIVI_Depth_model_pi") ); // ocpn_plugin.h

    dmDrawer = new dmDepthModelDrawer();

    wxFileConfig* pFileConf = GetOCPNConfigObject(); // ocpn_plugin.h, configuration file
    // opencpn canvas pointer, to be the parent for UI dialog
    m_parent_window = GetOCPNCanvasWindow(); // ocpn_plugin.h

    // Save the icon bitmap of this plugin as icon. About dialog needs this.
    m_icon = new wxIcon(); // Moved to initialization list
    m_icon->CopyFromBitmap(*_img_LIVI_Depth_model);

    dialog = new Dlg(m_parent_window);
    dialog->plugin = this;
    dialog->SetAboutInfo();

    m_pconf = new dmConfigHandler(pFileConf, dialog);
    colourfileHandler = new dmColourfileHandler(m_pconf, *GetpSharedDataLocation());

    bool success = m_pconf->LoadConfig(); // config related to this plugin.
    PushConfigToUI();
    dmDrawer->setDataset(m_pconf->fileImport.filePath);
    dmDrawer->setChartDrawType(m_pconf->colour.getChartType());
    dmDrawer->setColourSchema(m_pconf->colour.getColouringType());
    dmDrawer->setColourConfigurationFile(m_pconf->colour.userColourConfPath);

    //    This PlugIn needs a toolbar icon, so request its insertion
    if (m_pconf->general.m_bLIVI_Depth_modelShowIcon)
    {
        pluginToolId = InsertPlugInTool(_T(""),
            _img_LIVI_Depth_model, _img_LIVI_Depth_model, wxITEM_CHECK,
            _("Depth model"), _(""), NULL,
            LIVI_DEPTH_MODEL_TOOL_POSITION, 0, this);
    }

    // Display size info is used to position the windoww
    int display_w, display_h;
    ::wxDisplaySize(&display_w, &display_h);
    m_pconf->general.SaveDispaySize(display_w, display_h);

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
    bool success = m_pconf->closeNDestroyDialog();
    if (success) { dialog = NULL; }

    bool newPluginState = false;
    m_pconf->SetPluginToolState(newPluginState);
    SetToolbarItemState(pluginToolId, newPluginState);

    m_pconf->SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window, to hide the dataset pic

    delete dmDrawer;

    return true;
}

int LIVI_Depth_model_pi::GetAPIVersionMajor()    {  return MY_API_VERSION_MAJOR;     }
int LIVI_Depth_model_pi::GetAPIVersionMinor()    {  return MY_API_VERSION_MINOR;     }

int LIVI_Depth_model_pi::GetPlugInVersionMajor() {  return PLUGIN_VERSION_MAJOR;     }
int LIVI_Depth_model_pi::GetPlugInVersionMinor() {  return PLUGIN_VERSION_MINOR;     }

wxBitmap* LIVI_Depth_model_pi::GetPlugInBitmap() {  return _img_LIVI_Depth_model_pi; }

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
* This will be called upon enabling a PlugIn via the user Dialog.
* Chance to setup any default options and behavior.
* Set any variable loadable from (possible Preferences window, and)
* config file(s).
*/
void LIVI_Depth_model_pi::SetDefaults(void)
{
    // TODO
}

// OpenCPN implementation seems to be incomplete or missing (2018-10-02, MKo)
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
* ?Implement this when depths can be drawn, and want e.g. to pass
* the user the depth value in a single point at cursor.?
*/
void LIVI_Depth_model_pi::SetCursorLatLon(double lat, double lon)
{
    //TODO
}

/**
* Passes the current viewport of the main program to this plugin.
* Save it to be able to use it, e.g. to refresh/ load again the maps
* that should be currently visible, or something.
* NOTE that RenderOverlay also passes the viewPort. So, this might not be
* required?
*/
void LIVI_Depth_model_pi::SetCurrentViewPort(PlugIn_ViewPort &vp)
{
    /*dmExtent extent =*/ dmDrawer->applyViewPortArea(vp);
}

/**
* @TODO NOT IMPLEMENTED yet.
* React to OCPN window resize. The other plugins I have watched onto,
* do not use this, so you may probably go well without.
* This could / must be implemented to keep the drawn dataset picture
* as optimal size, so the memory usage is minimal, but the pic large enough.
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
    if (NULL == dialog)
        return;

    DimeWindow(dialog);
}

/**
* Basics implemented already by DR pi.
* This function shows/hides the UI window, and initiates it if non-existant.
*/
void LIVI_Depth_model_pi::OnToolbarToolCallback(int id)
{
    if (m_pconf == NULL) 
    {
        wxFileConfig* confFile = GetOCPNConfigObject(); // Get the configuration file contents;

        dialog = new Dlg(m_parent_window);
        dialog->plugin = this;

        m_pconf = new dmConfigHandler(confFile, dialog);
    }
    else if(m_pconf->getDialog()==NULL)
    {
        dialog = new Dlg(m_parent_window);
        dialog->plugin = this;
        m_pconf->setDialog(dialog);
    }

    // Toggle : update toggle state, and window show status
    bool pluginShown = m_pconf->TogglePluginToolState();
    if(pluginShown)
    {
        dialog->Move(m_pconf->general.dialogXY);
        dialog->Fit(); 
        dialog->Show();
    }
    else
    {   dialog->Hide();  }

    // Toggle is handled by the toolbar but we must keep plugin manager
    // b_toggle updated to ensure correct status upon toolbar rebuild
    SetToolbarItemState(pluginToolId, pluginShown);

    //RequestRefresh(m_parent_window); // refresh main window
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
bool LIVI_Depth_model_pi::RenderOverlay(wxDC& dc, PlugIn_ViewPort* vp)
{
    bool success = true;
    //success = dmDrawer->reCalculateDepthModelBitmap(*vp);

    bool exception = false;
    try
    {
        success = dmDrawer->drawDepthChart(dc, *vp);
    }
    catch (std::string ex)
    {
        exception = true;
        setErrorToUI("Problem in drawing the picture.\n"
                     "Look for the details in the OCPN log.");
        success = false;
    }
    if (exception)
        dialog->SetPictureImportErrorText("Problem in drawing the picture.");

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
* This is called, if this plugin has stated it INSTALLS_TOOLBOX_PAGE
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

/**
* @since opencpn_plugin_112
* Implement if want to get e.g. mouse clicks, to see at what position
* did the user click.
*/
bool LIVI_Depth_model_pi::MouseEventHook(wxMouseEvent &event)
{
    // TODO
    return false; // not handled; event handling is continued after this function
}


//// LIVI additions ////

wxString LIVI_Depth_model_pi::GetLongPluginVersionString() {
    std::string versionStr =
        PLUGIN_VERSION_MAJOR + '.' + PLUGIN_VERSION_MINOR +
        ',' + ' ' + PLUGIN_VERSION_DATE;
    return _("LIVI Depth model plugin, version "+ versionStr);
}

wxString LIVI_Depth_model_pi::GetCopyright() {
    return _("@ 2018, LIVI & Sitowise");
}

void LIVI_Depth_model_pi::OnDepthModelDialogClose()
{
    const bool state = false;
    m_pconf->SetPluginToolState(state);
    SetToolbarItemState(pluginToolId, state);
    dialog->Hide();

    RequestRefresh(m_parent_window); // refresh main window

    m_pconf->SaveConfig();
}

void LIVI_Depth_model_pi::OnImageFileChange(wxFileName fname)
{
    bool success = dmDrawer->setDataset(fname);
    if(success)
    {
        m_pconf->fileImport.filePath = fname;
        m_pconf->SaveConfig();
    }
    else
    {
        setErrorToUI("Could not open the given chart image file.");
    }
}

void LIVI_Depth_model_pi::OnGenerateImage(wxFileName fullFileName)
{
    // Check existance of the file
    wxString path = fullFileName.GetFullPath();
    wxFile file(path, wxFile::read); // also opens the file, if it exists!
    if (!file.Exists(path))
    {
        setErrorToUI("The chart image file cannot be found");
        return;
    }
    else if(file.IsOpened())
    {   file.Close();    }

    setInfoToUI("Setting chart image type options");

    bool exception = false;
    bool success = true;
    try {
        if(dmDrawer->getChartDrawType() == COLOR_RELIEF)
        {
            setInfoToUI("Setting colouring options");

            DM_colourType colouringType = dmDrawer->getColourSchema();
            success = colourfileHandler->SaveConfFileOfUISelection(
                colouringType); // Save, to get the current options in use

            wxFileName colorFile = colourfileHandler->GetConfFileOfUISelection(colouringType);
            if (!colorFile.IsOk())
            {
                setErrorToUI("Could not retrieve the colour definitions.");
                return;
            }

            success = dmDrawer->setColourConfigurationFile(colorFile);
            if (!success)
            {
                setErrorToUI("Failure at instantiating the colour configuration.");
                return;
            }

        }   // if

        setInfoToUI("Reading and projecting chart image to World Mercator");
        success &= dmDrawer->openDataset(fullFileName);
        if (!success)
        {
            setErrorToUI("Could not load the file as a chart image.");
            return;
        }

        setInfoToUI("Chart image successfully opened.");
    }
    catch (std::string exStr)
    {
        setInfoToUI(exStr);
        exception = true;
    }

    RequestRefresh(m_parent_window); // refresh main window

    m_pconf->fileImport.filePath = fullFileName;
    m_pconf->SaveConfig();

}

void LIVI_Depth_model_pi::OnChartTypeChange(int selectionId)
{
    DM_visualization chartType = to_dmVisualizationType(selectionId);
    bool success = dmDrawer->setChartDrawType(chartType);
    if (success)
    {
        m_pconf->colour.setChartType(chartType);
        m_pconf->SaveConfig();
    }
    else
    {
        setErrorToUI("Undefined chart visualization type setting.");
    }
}

void LIVI_Depth_model_pi::OnColourSchemaChange(int selectionId)
{
    DM_colourType colType = to_dmColourType(selectionId);
    bool success = dmDrawer->setColourSchema(colType);
    if (success)
    {
        m_pconf->colour.setColouringType(colType);
        m_pconf->SaveConfig();
    }
    else
    {
        setErrorToUI("Undefined colouring type setting.");
    }
}

void LIVI_Depth_model_pi::OnUserColourFileChange(wxFileName fullFileName)
{
    bool success = dmDrawer->setColourConfigurationFile(fullFileName);
    if (success)
    {
        m_pconf->colour.userColourConfPath = fullFileName;
        m_pconf->SaveConfig();
    }
    else
    {
        setErrorToUI("Could not find the given user's colour definition file.");
    }
}


//// private ////

/**
* Sets the colouring information, as well as the limiting depths,
* from the m_customColours of the m_conf to the UI.
*/
void LIVI_Depth_model_pi::PushConfigToUI(void)
{
    dialog->SetDepthChartFileName(m_pconf->fileImport.filePath);
    dialog->SetUserColourConfigurationFileName(m_pconf->colour.userColourConfPath);

    for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
        dialog->SetCustomColor(i, m_pconf->colour.getColour(i));
    }
    for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
        dialog->SetCustomLevel(i, m_pconf->colour.getDepth(i));
    }

    for (int i = 0; i < 2; i++) {
        dialog->SetTwoColours(i, m_pconf->colour.getTwoColour(i));
    }
    dialog->SetDividingLevel(m_pconf->colour.getTwoColoursDepth());
}

/**
* Retrieves the colouring information, as well as the limiting depths,
* from the UI to the m_customColours of the m_conf.
*/
void LIVI_Depth_model_pi::PullConfigFromUI(void)
{
    m_pconf->fileImport.filePath = dialog->GetDepthChartFileName();
    m_pconf->colour.userColourConfPath = dialog->GetUserColourConfigurationFileName();

    for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
        m_pconf->colour.setColour(i, dialog->GetCustomColor(i));
    }
    for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
        m_pconf->colour.setDepth(i, dialog->GetCustomLevel(i));
    }

    for (int i = 0; i < 2; i++) {
        m_pconf->colour.setTwoColour(i, dialog->GetTwoColours(i));
    }
    m_pconf->colour.setTwoColoursDepth(dialog->GetDividingLevel());
}

void LIVI_Depth_model_pi::setInfoToUI(std::string str)
{
    dialog->SetPictureImportInfoText(str);
}

void LIVI_Depth_model_pi::setErrorToUI(std::string str)
{
    dialog->SetPictureImportErrorText(str);
}

DM_visualization LIVI_Depth_model_pi::to_dmVisualizationType(int chartTypeId)
{
    switch (chartTypeId)
    {
    case DM_viz_HILLSHADE:      { return HILLSHADE;                 break; }
    case DM_viz_NONE:           { return NONE;                      break; }
    case DM_viz_COLOR_RELIEF:   { return COLOR_RELIEF;              break; }
    default:                    { return VISUALIZATION_UNDEFINED;   break; }
    }
}

DM_colourType LIVI_Depth_model_pi::to_dmColourType(int colouringChoiceId)
{
    switch (colouringChoiceId)
    {
    case DM_viz_USER_FILE:          { return COLOUR_USER_FILE;   break; }
    case DM_viz_FIVE_DEPTH_RANGES:  { return COLOUR_FIVE_RANGES; break; }
    case DM_viz_SLIDING_COLOUR:     { return COLOUR_SLIDING;     break; }
    case DM_viz_TWO_DEPTH_RANGES:   { return COLOUR_TWO_RANGES;  break; }
    default:                        { return COLOUR_UNDEFINED;   break; }
    }
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