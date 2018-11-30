#pragma once

#ifndef _DM_CONFIG_HANDLER_
#define _DM_CONFIG_HANDLER_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/filename.h>
#include "dm_API.h" // DM_colourType, DM_visualization
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
    : dialogXY(0,0), dialogSize(0,0), displaySize(100,100)
    , depthViewerDialogXY(0, 0)
    {   this->confFile = confFile;    }

    wxPoint dialogXY;
    wxPoint depthViewerDialogXY;
    wxSize  dialogSize;
  //wxSize  depthViewerDialogSize;
    wxSize  displaySize;
    bool    m_bLIVI_Depth_modelShowIcon;
    bool    m_bDepthsViewerShowIcon;

    virtual bool load() override;
    virtual bool save() override;

    void SaveDispaySize(int w, int h);
    void SetDialogXY(int x, int y);
    void SetDialogSize(int w, int h);
};

#define DM_NUM_CUSTOM_COL 5 // Number of custom colors
#define DM_NUM_CUSTOM_DEP 4 // Number of custom border depths. Must be equal to DM_NUM_CUSTOM_COL-1.

struct DMColorOptionConfig : dm_configAPI {

    DMColorOptionConfig(wxFileConfig* confFile)
    {   this->confFile = confFile;    }

    DM_visualization    chartType;
    DM_colourType       colouringType;
    // Filepath and filename of the user given colour conf file file
    wxFileName      userColourConfPath;

    wxColor m_customColours[DM_NUM_CUSTOM_COL]; // 0: col. for always too shallow, 4: col. for always deep enough
    double  m_customDepths[DM_NUM_CUSTOM_DEP]; // 0: level of too shallow, 3: level of always deep enough

    /*
    wxColor m_SlidingColours[2];    // 0:colour for too shallows, 1: colour for deep enoughs
    int     m_SlidingDepths[2];     // 0:depth of always too shallow, 1: depth of always deep enough
    int     m_SlidingSteps;         // Number of colour steps from deepest to shallowest
    */
    wxColor m_twoColours[2]; // 0: col. for too shallow, 1: col. for deep enough
    double  m_twoColoursDepth; // dividing depth

    double          hillshadeAltitude;
    double          hillshadeAzimuth;
    unsigned int    hillshadeTransparency;
    double          hillshadeZfactor;

    virtual bool load() override;
    virtual bool save() override;

    DM_visualization getChartType()                          { return chartType;            }
    void             setChartType(DM_visualization chartType){ this->chartType = chartType; }

    DM_colourType   getColouringType()                      { return colouringType;          }
    void            setColouringType(DM_colourType colType) { this->colouringType = colType; }

    // five colour range settings
    wxColor getColour(int i)             { return m_customColours[i];       }
    void    setColour(int i, wxColor col){ this->m_customColours[i] = col;  }

    double  getDepth(int i)              { return m_customDepths[i];        }
    void    setDepth(int i, double depth){ this->m_customDepths[i] = depth; }

    // two colour range settings
    wxColor getTwoColour(int i)              { return m_twoColours[i];      }
    void    setTwoColour(int i, wxColor col) { this->m_twoColours[i] = col; }

    double  getTwoColoursDepth()             { return m_twoColoursDepth;        }
    void    setTwoColoursDepth(double depth) { this->m_twoColoursDepth = depth; }

    // hillshade settings
    double  getHillshadeAltitude()                      {   return hillshadeAltitude;                   }
    void    setHillshadeAltitude(double altitudeInInDeg){   this->hillshadeAltitude = altitudeInInDeg;  }

    double  getHillshadeAzimuth()                   {   return hillshadeAzimuth;                }
    void    setHillshadeAzimuth(double azimuthInDeg){   this->hillshadeAzimuth = azimuthInDeg;  }

    unsigned int getHillshadeTransparency() {   return hillshadeTransparency;                   }
    void         setHillshadeTransparency(unsigned int transparency)
                                            {   this->hillshadeTransparency = transparency;     }

    double  getHillshadeZfactor()               {   return hillshadeZfactor;            }
    void    setHillshadeZfactor(double zFactor) {   this->hillshadeZfactor = zFactor;   }

private:
    DM_visualization    stringToChartType(wxString chartTypeString);
    DM_colourType   stringToColouringType(wxString colTypeString);
};

struct DMFileImportConfig : dm_configAPI {

    DMFileImportConfig(wxFileConfig* confFile)
    {   this->confFile = confFile;    }

    // Filepath to the imported file
    wxFileName filePath;

    virtual bool load() override;
    virtual bool save() override;
};

struct DMWaterLevelConfig : dm_configAPI
{
    DMWaterLevelConfig(wxFileConfig* confFile)
    {    this->confFile = confFile;    }

    double  m_currentWaterLevel;
    double  m_verticalReferenceSystemOffset;

    virtual bool load() override;
    virtual bool save() override;

    double  getCurrentWaterLevel()               { return m_currentWaterLevel;     }
    void    setCurrentWaterLevel(double levelInM){ m_currentWaterLevel = levelInM; }

    double  getVerticalReferenceSystemOffset()
    {   return m_verticalReferenceSystemOffset;             }
    void    setVerticalReferenceSystemOffset(double vrsOffsetInM)
    {   m_verticalReferenceSystemOffset = vrsOffsetInM;     }
};

class dmConfigHandler
{
public:
    dmConfigHandler(
        wxFileConfig* confFile, // change this to reference, to avoid accidentally deleting, 
        Dlg* pluginDialog=NULL)
      : m_pconfig(confFile), m_pDialog(pluginDialog),
        general(confFile),colour(confFile), fileImport(confFile), waterLevel(confFile),
        showDepthModel(false), showDepthsViewer(false)
    { 
        /*if debug */assert(confFile);
        /* else */ // write error to log?
    }

    bool LoadConfig();
    bool SaveConfig();

    Dlg* getDialog();
    bool setDialog(Dlg* pluginDialog);
    bool closeNDestroyDialog();

    bool SetPluginToolState(bool state);
    bool TogglePluginToolState();

    bool SetDepthsViewerToolState(bool state);
    bool ToggleDepthsViewerToolState();

    DMGeneralConfig     general;
    DMColorOptionConfig colour;
    DMFileImportConfig  fileImport;
    DMWaterLevelConfig  waterLevel;
    Dlg*                m_pDialog;

private:
    /** OpenCPN config file for accessing config options in .ini file */
    wxFileConfig*       m_pconfig;

    bool                showDepthModel;
    bool                showDepthsViewer;

    wxFileName          pluginConfigPath; // full config file path name
};

#endif _DM_CONFIG_HANDLER_