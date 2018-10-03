#include "dmConfigHandler.h"

#include <wx/fileconf.h>        // wxFileConfig
#include "LIVI_Depth_model_pi_UI_impl.h" // Dlg

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
    return success;
}

bool dmConfigHandler::SaveConfig(void)
{
    bool success = true;
    success &= m_pconfig->Write(wxT("ConfigFilePath"), pluginConfigPath.GetFullPath());

    return success;

}

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
