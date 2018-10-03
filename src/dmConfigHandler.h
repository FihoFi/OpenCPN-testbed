#pragma once

#ifndef _DM_CONFIG_HANDLER_
#define _DM_CONFIG_HANDLER_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include <wx/filename.h>
class wxFileConfig;

class dm_configAPI
{
    virtual bool load() = 0;
    virtual bool save() = 0;

protected:
    wxFileConfig* confFile;
};

#define DM_NUM_CUSTOM_COL 5 // Number of custom colors
#define DM_NUM_CUSTOM_DEP 4 // Number of custom border depths. Must be equal to DM_NUM_CUSTOM_COL-1.
struct DMColorOptionConfig : dm_configAPI {

    DMColorOptionConfig(wxFileConfig* confFile)
    {   this->confFile = confFile;    }

    wxColor m_customColours[DM_NUM_CUSTOM_COL]; // 0: col. for always too shallow, 4: col. for always deep enough
    double  m_customDepths[DM_NUM_CUSTOM_DEP]; // 0: level of too shallow, 3: level of always deep enough
    /*
    wxColor m_SlidingColours[2];    // 0:colour for too shallows, 1: colour for deep enoughs
    int     m_SlidingDepths[2];     // 0:depth of always too shallow, 1: depth of always deep enough
    int     m_SlidingSteps;         // Number of colour steps from deepest to shallowest
    */
    virtual bool load() override;
    virtual bool save() override;

    wxColor getColour(int i)             { return m_customColours[i]; }
    void    setColour(int i, wxColor col){ m_customColours[i] = col;  }

    double  getDepth(int i)              { return m_customDepths[i];  }
    void    setDepth(int i, double depth){ m_customDepths[i] = depth; }
};

class dmConfigHandler
{
public:
    dmConfigHandler(
        wxFileConfig* confFile) // change this to reference, to avoid accidentally deleting
      : m_pconfig(confFile)
        colour(confFile)
        showDepthModel(false)
    { 
        /*if debug */assert(confFile);
        /* else */ // write error to log?
    }

    bool LoadConfig();
    bool SaveConfig();

    DMColorOptionConfig colour;
private:
    /** OpenCPN config file for accessing config options in .ini file */
    wxFileConfig*       m_pconfig;

    bool                showDepthModel;

    wxFileName          pluginConfigPath; // full config file path name
};

#endif _DM_CONFIG_HANDLER_