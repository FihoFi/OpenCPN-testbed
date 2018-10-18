#pragma once

#ifndef _DM_CONFIG_HANDLER_
#define _DM_CONFIG_HANDLER_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/filename.h>
class Dlg;
class wxFileConfig;

class dm_configAPI
{
    virtual bool load() = 0;
    virtual bool save() = 0;

protected:
    wxFileConfig* confFile;
};

struct DMGeneralConfig : dm_configAPI {
    DMGeneralConfig(wxFileConfig* confFile)
    :   dialogXY(0,0), dialogSize(0,0), displaySize(100,100)
    {   this->confFile = confFile;    }

    wxPoint dialogXY;
    wxSize  dialogSize;
    wxSize  displaySize;
    bool    m_bLIVI_Depth_modelShowIcon;

    virtual bool load() override;
    virtual bool save() override;

    void SetDialogXY(int x, int y) {
        dialogXY.x = x;
        dialogXY.y = y;

        if ((dialogXY.x < 0) || (dialogXY.x > displaySize.GetWidth()))
            dialogXY.x = 5;
        if ((dialogXY.y < 0) || (dialogXY.y > displaySize.GetHeight()))
            dialogXY.y = 5;
    }

    void SetLIVIDepthModelDialogSize(int w, int h)
    { dialogSize.SetWidth(w); dialogSize.SetHeight(h); }

    void SaveDispaySize(int w, int h)
    {   displaySize.SetWidth(w); displaySize.SetHeight(h);    }

};

#define DM_NUM_CUSTOM_COL 5 // Number of custom colors
#define DM_NUM_CUSTOM_DEP 4 // Number of custom border depths. Must be equal to DM_NUM_CUSTOM_COL-1.

struct DMColorOptionConfig : dm_configAPI {

    DMColorOptionConfig(wxFileConfig* confFile)
    {   this->confFile = confFile;    }

    // Filepath and filename of the user given colour conf file file
    wxFileName userColourConfPath;

    wxColor m_customColours[DM_NUM_CUSTOM_COL]; // 0: col. for always too shallow, 4: col. for always deep enough
    double  m_customDepths[DM_NUM_CUSTOM_DEP]; // 0: level of too shallow, 3: level of always deep enough

    /*
    wxColor m_SlidingColours[2];    // 0:colour for too shallows, 1: colour for deep enoughs
    int     m_SlidingDepths[2];     // 0:depth of always too shallow, 1: depth of always deep enough
    int     m_SlidingSteps;         // Number of colour steps from deepest to shallowest
    */
    wxColor m_twoColours[2]; // 0: col. for too shallow, 1: col. for deep enough
    double  m_twoColoursDepth; // dividing depth

    virtual bool load() override;
    virtual bool save() override;

    wxColor getColour(int i)             { return m_customColours[i]; }
    void    setColour(int i, wxColor col){ m_customColours[i] = col;  }

    double  getDepth(int i)              { return m_customDepths[i];  }
    void    setDepth(int i, double depth){ m_customDepths[i] = depth; }

    wxColor getTwoColour(int i)              { return m_twoColours[i]; }
    void    setTwoColour(int i, wxColor col) { m_twoColours[i] = col; }

    double  getTwoColoursDepth()             { return m_twoColoursDepth; }
    void    setTwoColoursDepth(double depth) { m_twoColoursDepth = depth; }
};

struct DMFileImportConfig : dm_configAPI {

    DMFileImportConfig(wxFileConfig* confFile)
    {   this->confFile = confFile;    }

    // Filepath to the imported file
    wxFileName filePath;

    // Min, and max coordinates where the picture in the filePath should be drawn
    double latMin, lonMin;
    double latMax, lonMax;

    virtual bool load() override;
    virtual bool save() override;
};

class dmConfigHandler
{
public:
    dmConfigHandler(
        wxFileConfig* confFile, // change this to reference, to avoid accidentally deleting, 
        Dlg* pluginDialog=NULL)
      : m_pconfig(confFile), m_pDialog(pluginDialog),
        general(confFile),colour(confFile), fileImport(confFile),
        showDepthModel(false)
    { 
        /*if debug */assert(confFile);
        /* else */ // write error to log?
    }

    bool LoadConfig();
    bool SaveConfig();

    Dlg* getDialog();
    bool setDialog(Dlg* pluginDialog);
    bool closeNDestroyDialog();

    bool SetPluginToolState(bool state) {
        showDepthModel = state;
        return showDepthModel;
    }
    bool TogglePluginToolState() {
        showDepthModel = !showDepthModel;
        return showDepthModel;
    }
    DMGeneralConfig     general;
    DMColorOptionConfig colour;
    DMFileImportConfig  fileImport;
    Dlg*                m_pDialog;

private:
    /** OpenCPN config file for accessing config options in .ini file */
    wxFileConfig*       m_pconfig;

    bool                showDepthModel;

    wxFileName          pluginConfigPath; // full config file path name
};

#endif _DM_CONFIG_HANDLER_