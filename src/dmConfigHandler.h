#pragma once

#ifndef _DM_CONFIG_HANDLER_
#define _DM_CONFIG_HANDLER_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

//#include <wx/filename.h>
class wxFileName;
class wxFileConfig;

class dmConfigHandler
{
public:
    dmConfigHandler(
        wxFileConfig* confFile) // change this to reference, to avoid accidentally deleting, 
      : m_pconfig(confFile)
        showDepthModel(false)
    { 
        /*if debug */assert(confFile);
        /* else */ // write error to log?
    }

    bool LoadConfig();
    bool SaveConfig();

private:
    /** OpenCPN config file for accessing config options in .ini file */
    wxFileConfig*       m_pconfig;

    bool                showDepthModel;

    wxFileName          pluginConfigPath; // full config file path name
};

#endif _DM_CONFIG_HANDLER_
