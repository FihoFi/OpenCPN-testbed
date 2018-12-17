#include "dmConfigHandler.h"

#include <wx/fileconf.h>        // wxFileConfig
#include "LIVI_Depth_model_pi_UI_impl.h" // Dlg

/* Loads config (ini) file entries of the Depth model. */
bool dmConfigHandler::LoadConfig(void)
{
    bool success = true;
    m_pconfig->SetPath(_T("/Settings/LIVI_Depth_model_pi"));

    wxString pluginConfPathStr;
    success &= m_pconfig->Read(wxT("ConfigFilePath"), &pluginConfPathStr);
    wxFileName pluginConfPath (pluginConfPathStr);
    if (pluginConfPath.FileExists())
    {
        pluginConfigPath = pluginConfPath;
    }

    success &= general.load();
    success &= colour.load();
    success &= fileImport.load();
    success &= waterLevel.load();

    return success;
}

/*  Saves data entries of the Depth model configurations to the
config settings. These are updated to the ini file at OpenCPN close. */
bool dmConfigHandler::SaveConfig(void)
{
    bool success = true;
    m_pconfig->SetPath(_T("/Settings/LIVI_Depth_model_pi"));

    success &= m_pconfig->Write(wxT("ConfigFilePath"), pluginConfigPath.GetFullPath());

    success &= general.save();
    success &= colour.save();
    success &= fileImport.save();
    success &= waterLevel.save();

    m_pconfig->Flush();

    return success;
}

Dlg* dmConfigHandler::getDialog()
{    return m_pDialog;    }

bool dmConfigHandler::setDialog(Dlg* pluginDialog)
{
    if (m_pDialog == pluginDialog)  // No need to set, if already set as the request.
    {   return true;    }

    if(!pluginDialog)               // Not setting a null. Must use closeNDestroy instead.
    {   return false;    }

    if (m_pDialog)                  // Destroy an existing first.
    {
        m_pDialog->Close(); 
        delete m_pDialog;
    }

    m_pDialog = pluginDialog;

    return true;
}

bool dmConfigHandler::closeNDestroyDialog()
{
    //    Record the dialog position
    if (m_pDialog != NULL)
    {
        //Capture dialog position
        wxPoint p = m_pDialog->GetPosition();
        general.SetDialogXY(p.x, p.y);

        m_pDialog->Close();
        delete m_pDialog;
        m_pDialog = NULL;

        return true;
    }
    else /*if (m_pDialog == NULL)*/
    {   return false;    }
}

bool DMGeneralConfig::load(void)
{
    if (confFile)
    {
        confFile->SetPath(wxT("/Settings/LIVI_Depth_model_pi"));

        dialogXY.x = confFile->ReadLong(_T("DialogPosX"), 20L);
        dialogXY.y = confFile->ReadLong(_T("DialogPosY"), 20L);
        dialogSize.SetWidth(confFile->ReadLong(_T("DialogSizeW"), 20L));
        dialogSize.SetHeight(confFile->ReadLong(_T("DialogSizeH"), 20L));
        depthViewerDialogXY.x = confFile->ReadLong(_T("depthViewerDialogPosX"), 20L);
        depthViewerDialogXY.y = confFile->ReadLong(_T("depthViewerDialogPosY"), 20L);

        m_bLIVI_Depth_modelShowIcon  = confFile->ReadBool(_T("ShowLIVI_Depth_modelIcon"), true);
        m_bDepthsViewerShowIcon      = confFile->ReadBool(_T("ShowDepthsViewerIcon"),     true);
        return true;
    }
    return false;
}

bool DMGeneralConfig::save(void)
{
    if (confFile)
    {
        confFile->SetPath(wxT("/Settings/LIVI_Depth_model_pi"));

        confFile->Write(_T("DialogPosX"), dialogXY.x);
        confFile->Write(_T("DialogPosY"), dialogXY.y);
        confFile->Write(_T("DialogSizeW"), dialogSize.GetWidth());
        confFile->Write(_T("DialogSizeH"), dialogSize.GetHeight());
        confFile->Write(_T("depthViewerDialogPosX"), depthViewerDialogXY.x);
        confFile->Write(_T("depthViewerDialogPosY"), depthViewerDialogXY.y);

        confFile->Write(_T("ShowLIVI_Depth_modelIcon"), m_bLIVI_Depth_modelShowIcon);
        confFile->Write(_T("ShowDepthsViewerIcon"),     m_bDepthsViewerShowIcon);
        return true;
    }
    return false;
}

void DMGeneralConfig::SaveDispaySize(int w, int h)
{
    displaySize.SetWidth(w); displaySize.SetHeight(h);
}

void DMGeneralConfig::SetDialogXY(int x, int y) {
    dialogXY.x = x;
    dialogXY.y = y;

    if ((x < 0) || (x > displaySize.GetWidth())) { dialogXY.x = 5; }
    if ((y < 0) || (y > displaySize.GetHeight())) { dialogXY.y = 5; }
}

void DMGeneralConfig::SetDialogSize(int w, int h)
{   dialogSize.SetWidth(w); dialogSize.SetHeight(h);    }


void DMGeneralConfig::SetDepthsViewerDialogXY(int x, int y) {
    depthViewerDialogXY.x = x;
    depthViewerDialogXY.y = y;

    if ((depthViewerDialogXY.x < 0) || (depthViewerDialogXY.x > displaySize.GetWidth()))
        depthViewerDialogXY.x = 5;
    if ((depthViewerDialogXY.y < 0) || (depthViewerDialogXY.y > displaySize.GetHeight()))
        depthViewerDialogXY.y = 5;
}

bool DMColorOptionConfig::load(void)
{
    bool success = true;    // TODO success of Read functions has not been checked
    if (confFile)
    {
        confFile->SetPath(_T("/Settings/LIVI_Depth_model_pi/Colour"));

        std::string strCh = confFile->Read(_T("ChartType"));
        chartType = stringToChartType(strCh);
        if (chartType == VISUALIZATION_UNDEFINED)
            chartType = COLOR_RELIEF;

        std::string strCol = confFile->Read(_T("ColouringType"));
        colouringType = stringToColouringType(strCol);
        if (colouringType == COLOUR_UNDEFINED)
            colouringType = COLOUR_FIVE_RANGES;

        static std::string defaults_for_five[5] = { "#ff0000", "#ffc4e4", "#ffffff", "#80c4ff", "#0000ff" };
        for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
            std::string colour = confFile->Read(_T("CustomColour") + std::to_string(i));
            if (colour.length() == 0)
                colour = defaults_for_five[i]; // default, if no color found

            unsigned char* valChar = (unsigned char*)colour.c_str();
            m_customColours[i] = wxColour(valChar);
        }
        for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
            std::string str = confFile->Read(_T("CustomDepth" + std::to_string(i)));
            if (str.length() == 0)
                str = std::to_string(-20); // -20m if no depth found
            m_customDepths[i] = std::stoi(str);
        }

        static std::string defaults_for_two[2] = { "#ff0000", "#0000ff" };
        for (int i = 0; i < 2; i++) {
            std::string colour = confFile->Read(_T("TwoColour") + std::to_string(i));
            if (colour.length() == 0)
                colour = defaults_for_two[i]; // default, if no color found

            unsigned char* valChar = (unsigned char*)colour.c_str();
            m_twoColours[i] = wxColour(valChar);
        }
        std::string str = confFile->Read(_T("TwoColourDepth"));
        if (str.length() == 0)
            str = std::to_string(-20); // -20m if no depth found
        m_twoColoursDepth = std::stoi(str);

        hillshadeAltitude   = confFile->ReadDouble(_T("HillshadeAltitude_0to90"),       45);
        hillshadeAzimuth    = confFile->ReadDouble(_T("HillshadeAzimuth_0to360"),       315);
        hillshadeTransparency   = 
               (unsigned int)confFile->ReadLong(_T("HillshadeTransparency_0to255"),     128);
        hillshadeZfactor    = confFile->ReadDouble(_T("HillshadeVerticalExaggeration"), 10);
      //hillshadeZfactor    = confFile->ReadDouble(_T("HillshadeZfactor"),              10);    // alt name

        wxString colourConfPathStr;
        success &= confFile->Read(wxT("UserColourConfPath"), &colourConfPathStr);
        wxFileName colourConfFilePath(colourConfPathStr);
        if (colourConfFilePath.FileExists())
        {
            userColourConfPath = colourConfFilePath;
        }
    }
    return success;
}

bool DMColorOptionConfig::save(void)
{
    bool success = true;
    if (confFile)
    {
        confFile->SetPath(_T("/Settings/LIVI_Depth_model_pi/Colour"));

        success &= confFile->Write( _T("ChartType"),
                                wxString(chartTypeToString(chartType)));
        success &= confFile->Write( _T("ColouringType"),
                                wxString(colouringTypeToString(colouringType)));

        for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
            success &= confFile->Write(_T("CustomColour") + std::to_string(i),
                m_customColours[i].GetAsString(wxC2S_HTML_SYNTAX));
        }
        for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
            success &= confFile->Write(_T("CustomDepth" + std::to_string(i)),
                m_customDepths[i]);
        }

        for (int i = 0; i < 2; i++) {
            success &= confFile->Write(_T("TwoColour") + std::to_string(i),
                m_twoColours[i].GetAsString(wxC2S_HTML_SYNTAX));
        }
        success &= confFile->Write(_T("TwoColourDepth"), m_twoColoursDepth);

        success &= confFile->Write(_T("HillshadeAltitude_0to90"),       hillshadeAltitude);
        success &= confFile->Write(_T("HillshadeAzimuth_0to360"),       hillshadeAzimuth);
        success &= confFile->Write(_T("HillshadeTransparency_0to255"), (long)hillshadeTransparency);
        success &= confFile->Write(_T("HillshadeVerticalExaggeration"), hillshadeZfactor);
      //success &= confFile->Write(_T("HillshadeZfactor"),              hillshadeZfactor);    // alt name

        success &= confFile->Write(_T("UserColourConfPath"), userColourConfPath.GetFullPath());

        return success;
    }
    else
        return false;
}

bool DMWaterLevelConfig::load(void)
{
    //bool success = true;    // TODO success of Read functions has not been checked
    if (confFile)
    {
        confFile->SetPath(_T("/Settings/LIVI_Depth_model_pi/Levels"));

        m_currentWaterLevel             = confFile->ReadDouble(_T("CurrentWaterLevel"), 0);
        m_verticalReferenceSystemOffset = confFile->ReadDouble(_T("VerticalRefSystemZ"),0);

        return true;
    }
    else
        return false;
}

bool DMWaterLevelConfig::save(void)
{
    bool success = true;
    if (confFile)
    {
        confFile->SetPath(_T("/Settings/LIVI_Depth_model_pi/Levels"));

        success &= confFile->Write(_T("CurrentWaterLevel"), m_currentWaterLevel);
        success &= confFile->Write(_T("VerticalRefSystemZ"), m_verticalReferenceSystemOffset);
        return success;
    }
    else
        return false;
}


DM_visualization DMColorOptionConfig::stringToChartType(wxString chartTypeString) {
    if      (chartTypeString.Contains("Plain")        ) { return NONE;                      }
    else if (chartTypeString.Contains("Hillshade")    ) { return HILLSHADE;                 }
    else if (chartTypeString.Contains("Color_relief") ) { return COLOR_RELIEF;              }
    else                                                { return VISUALIZATION_UNDEFINED;   }
}

DM_colourType DMColorOptionConfig::stringToColouringType(wxString colTypeString)
{
    if      (colTypeString.Contains("User_file")  ) { return COLOUR_USER_FILE;   }
    else if (colTypeString.Contains("Five_ranges")) { return COLOUR_FIVE_RANGES; }
    else if (colTypeString.Contains("Sliding")    ) { return COLOUR_SLIDING;     }
    else if (colTypeString.Contains("Two_ranges") ) { return COLOUR_TWO_RANGES;  }
    else                                            { return COLOUR_UNDEFINED;   }
}

bool DMFileImportConfig::load()
{
    bool success = true;
    if (confFile)
    {
        confFile->SetPath(_T("/Settings/LIVI_Depth_model_pi/FileImport"));

        wxString fileImportPath;
        success &= confFile->Read(wxT("FileImport_fullpathname"), &fileImportPath);
        wxFileName fname(fileImportPath);
        if (fname.DirExists())
        {
            filePath = fname;
        }
    }
    return success;
}

bool DMFileImportConfig::save(void)
{
    bool success = true;
    if (confFile)
    {
        confFile->SetPath(_T("/Settings/LIVI_Depth_model_pi/FileImport"));

        success &= confFile->Write(wxT("FileImport_fullpathname"), filePath.GetFullPath());
    }
    return success;
}


bool dmConfigHandler::SetPluginToolState(bool state) {
    showDepthModel = state;
    return showDepthModel;
}

bool dmConfigHandler::TogglePluginToolState() {
    showDepthModel = !showDepthModel;
    return showDepthModel;
}

bool dmConfigHandler::SetDepthsViewerToolState(bool state) {
    showDepthsViewer = state;
    return showDepthsViewer;
}

bool dmConfigHandler::ToggleDepthsViewerToolState() {
    showDepthsViewer = !showDepthsViewer;
    return showDepthsViewer;
}