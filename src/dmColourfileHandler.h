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


    bool            SaveConfFileOfUISelection(DM_colourType colourOption);
    wxFileName      GetConfFileOfUISelection(DM_colourType colourOption);

  //wxFileName      GetUsersColorConfFile();
    bool            SaveFiveColorConfToFile();
    bool            SaveSlidingColorConfToFile();
    bool            SaveTwoColorConfToFile();

private:
    bool            SaveColorConfToFile(wxFileName &confPath,
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
