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
    success &= colour.load();
    return success;
}

bool dmConfigHandler::SaveConfig(void)
{
    bool success = true;
    m_pconfig->SetPath(_T("/Settings/LIVI_Depth_model_pi"));

    success &= m_pconfig->Write(wxT("ConfigFilePath"), pluginConfigPath.GetFullPath());

    success &= colour.save();

    return success;

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
    }
    return success;
}
