#pragma once

#ifndef _DM_COLOURFILE_HANDLER_H_
#define _DM_COLOURFILE_HANDLER_H_

#include <wx/fileconf.h>    // wxFileName

class dmConfigHandler;
enum DM_colourType;

class dmColourfileHandler
{
public:
    dmColourfileHandler(dmConfigHandler* confH, wxString tempDataDirectoryPath);

    dmColourfileHandler& operator= (const dmColourfileHandler& other);

    bool            GetConfFileOfType(DM_colourType colourOption, wxFileName& colorFile);

private:
  //wxFileName      GetUsersColorConfFile();
    bool            GenerateConfFileOfType(DM_colourType colourOption);
    bool            GenerateFiveColorConfFile();
    bool            GenerateSlidingColorConfFile();
    bool            GenerateTwoColorConfFile();

    bool            GenerateColorConfFile(wxFileName &confPath,
                        const wxString fileName, const wxString confText);

    wxString        GetFiveColourDepthColourWks();
    wxString        GetSlidingColourDepthColourWks();
    wxString        GetTwoColourDepthColourWks();

    wxFileName      fiveColoursFileName;
    wxFileName      slidingColoursFileName;
    wxFileName      twoColoursFileName;

    dmConfigHandler* m_pconf;
    wxString        tempDataDirectoryPath;
};

#endif _DM_COLOURFILE_HANDLER_H_
