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

       dialogXY.x = confFile->Read(_T("DialogPosX"), 20L);
       dialogXY.y = confFile->Read(_T("DialogPosY"), 20L);

        confFile->Write(wxT("ShowLIVI_Depth_modelIcon"), m_bLIVI_Depth_modelShowIcon);
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

        confFile->Write(wxT("ShowLIVI_Depth_modelIcon"), m_bLIVI_Depth_modelShowIcon);
        return true;
    }
    return false;
}


bool DMColorOptionConfig::load(void)
{
    bool success = true;    // TODO success of Read functions has not been checked
    if (confFile)
    {
        confFile->SetPath(_T("/Settings/LIVI_Depth_model_pi/Colour"));
        for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
            std::string colour = confFile->Read(_T("CustomColour") + std::to_string(i));
            if (sizeof(colour) == 0)
                colour = "#f0f0f0"; // grey as default, if no color found
                                    // #ff0000, #ffc4e4, #ffffff, #80c4ff, #0000ff

            unsigned char* valChar = (unsigned char*)colour.c_str();
            m_customColours[i] = wxColour(valChar);
        }

        for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
            std::string str = confFile->Read(_T("CustomDepth" + std::to_string(i)));
            if (str.length() == 0)
                str = std::to_string(DM_NUM_CUSTOM_DEP * 5); // grey as default, if no color found
            m_customDepths[i] = std::stoi(str);
        }

        wxString colourConfPathStr;
        success &= confFile->Read(wxT("ColourConfPath"), &colourConfPathStr);
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

        for (int i = 0; i < DM_NUM_CUSTOM_COL; i++) {
            success &= confFile->Write(wxT("CustomColour") + std::to_string(i),
                m_customColours[i].GetAsString(wxC2S_HTML_SYNTAX));
        }
        for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
            success &= confFile->Write(wxT("CustomDepth" + std::to_string(i)), m_customDepths[i]);
        }

        success &= confFile->Write(wxT("ColourConfPath"), userColourConfPath.GetFullPath());

        return true;
    }
    else
        return false;
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

        std::string str = confFile->Read(wxT("LatMin"));
        if (str.length() == 0)
        {    /* todo Write an error message to the log file */
        }
        else { latMin = std::stoi(str); }

        str = confFile->Read(wxT("LatMax"));
        if (str.length() == 0)
        {    /* todo Write an error message to the log file */
        }
        else { latMax = std::stoi(str); }

        str = confFile->Read(wxT("LonMin"));
        if (str.length() == 0)
        {    /* todo Write an error message to the log file */
        }
        else { lonMin = std::stoi(str); }

        str = confFile->Read(wxT("LonMax"));
        if (str.length() == 0)
        {    /* todo Write an error message to the log file */
        }
        else { lonMax = std::stoi(str); }
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

        success &= confFile->Write(wxT("LatMin"), latMin);
        success &= confFile->Write(wxT("LatMax"), latMax);
        success &= confFile->Write(wxT("LonMin"), lonMin);
        success &= confFile->Write(wxT("LonMax"), lonMax);
    }
    return success;
}