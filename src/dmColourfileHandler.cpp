#include "dmColourfileHandler.h"
#include "dmConfigHandler.h"    // some #defines
#include "dm_API.h" // enum DM_colourType

class dmConfigHandler;

dmColourfileHandler::dmColourfileHandler(dmConfigHandler* confH, wxString tempDataDirectoryPath)
    : fiveColoursFileName()
    , slidingColoursFileName()
    , twoColoursFileName()
    , m_pconf(confH)
    , tempDataDirectoryPath(tempDataDirectoryPath)
{}


dmColourfileHandler& dmColourfileHandler::operator=(const dmColourfileHandler& other)
{
    this->fiveColoursFileName       = other.fiveColoursFileName;
    this->slidingColoursFileName    = other.slidingColoursFileName;
    this->twoColoursFileName        = other.twoColoursFileName;

    return *this;
}

//wxFileName dmColourfileHandler::GetUsersColorConfFile()
//{
//    return m_pconf->colour.userColourConfPath;
//    //return dialog->GetUserColourConfigurationFileName();
//}

bool dmColourfileHandler::SaveConfFileOfUISelection(DM_colourType colourOption)
{
    bool success = true;
    switch (colourOption)
    {
    case COLOUR_USER_FILE:      { break;                                          }
    case COLOUR_FIVE_RANGES:    { success &= SaveFiveColorConfToFile();    break; }
    case COLOUR_SLIDING:        { success &= SaveSlidingColorConfToFile(); break; }
    case COLOUR_TWO_RANGES:     { success &= SaveTwoColorConfToFile();     break; }
    default:                    { success = false;                         break; }
    }
    return success;
}

wxFileName dmColourfileHandler::GetConfFileOfUISelection(DM_colourType colourOption)
{
    switch (colourOption)
    {
    case COLOUR_USER_FILE:      { return m_pconf->colour.userColourConfPath;
                                /*return GetUsersColorConfFile();*/  break; }
    case COLOUR_FIVE_RANGES:    { return fiveColoursFileName;      break; }
    case COLOUR_SLIDING:        { return slidingColoursFileName;   break; }
    case COLOUR_TWO_RANGES:     { return twoColoursFileName;       break; }
    default:                    { return wxFileName("");           break; }
    }
}


bool dmColourfileHandler::SaveFiveColorConfToFile()
{
    wxString confText = GetFiveColourDepthColourWks();
    return SaveColorConfToFile(fiveColoursFileName, _T("five_colour_set.txt"), confText);
}

bool dmColourfileHandler::SaveSlidingColorConfToFile()
{
    wxString confText = GetSlidingColourDepthColourWks();
    return SaveColorConfToFile(slidingColoursFileName, _T("sliding_colour_set.txt"), confText);
}

bool dmColourfileHandler::SaveTwoColorConfToFile()
{
    wxString confText = GetTwoColourDepthColourWks();
    return SaveColorConfToFile(twoColoursFileName, _T("two_colour_set.txt"), confText);
}

bool dmColourfileHandler::SaveColorConfToFile(
    wxFileName &confFile, const wxString confFileName, const wxString confText)
{
    // Normal case: we have a functioning path available. Just write there
    wxString path = confFile.GetFullPath();

    // Try opening the file in the path
    wxFile file(path, wxFile::write);
    if (!file.Exists(path))
    {
        // No such path, set up a path
        wxFileName fn;
        fn.SetPath(tempDataDirectoryPath);
        fn.AppendDir(_T("plugins"));
        fn.AppendDir(_T("LIVI_Depth_model_pi"));
        fn.AppendDir(_T("colour_files"));
        fn.SetFullName(confFileName);

        bool success = fn.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
        if (success)
            confFile = fn;
        else
            return false;   // Could not create the path. This is bad.
    }
    path = confFile.GetFullPath();

    // Try opening the file again
    file.Open(path, wxFile::write);
    if (file.IsOpened())
    {
        bool success = file.Write(confText);
        file.Close();

        if (success)
            return true;
    }

    return false;
}


/**
* Generates a well known string (wks) about colour settings, telling
* the colouring options for GDAL colour_relief.
* Wks format is, for every row, like this:
* <depth_value> <Red> <Green> <Blue> <Alpha>
* We are going to use the nearest matching colour colouring option,
* so we insert two colour definitions near each depth value, small depth
* amount to each "side", so the nearest colour will be the wanted one
* at any depth level.
*/
wxString dmColourfileHandler::GetFiveColourDepthColourWks()
{
    static double nci = 0.0001; // Nearest colour tweak. Number in meters.

    // Default opaqueness values for each depth range. Less opaqueness = more
    // transparency for deeper water = smaller values. Values in [0...255]
    static int opaque_list[5] = { 128, 96, 64, 48, 32 };

    wxString wks_ColourSettings;
    wks_ColourSettings.append(wxString(_T("nv           0  0  0  0\r\n")));

    for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
        wks_ColourSettings.append(
            wxString::Format(_T("%f %i %i %i %i\r\n"),
                m_pconf->colour.m_customDepths[i] + nci,
                m_pconf->colour.m_customColours[i].Red(),
                m_pconf->colour.m_customColours[i].Green(),
                m_pconf->colour.m_customColours[i].Blue(),
                opaque_list[i])
        );
        wks_ColourSettings.append(
            wxString::Format(_T("%f %i %i %i %i\r\n"),
                m_pconf->colour.m_customDepths[i],
                m_pconf->colour.m_customColours[i + 1].Red(),
                m_pconf->colour.m_customColours[i + 1].Green(),
                m_pconf->colour.m_customColours[i + 1].Blue(),
                opaque_list[i + 1])
        );
    }

    return wks_ColourSettings;
}

wxString dmColourfileHandler::GetSlidingColourDepthColourWks()
{
    wxString wks_ColourSettings;

    //TODO
    wks_ColourSettings.append(wxString(_T("nv           0  0  0  0\r\n")));

    return wks_ColourSettings;
}

wxString dmColourfileHandler::GetTwoColourDepthColourWks()
{
    static double nci = 0.0001; // Nearest colour tweak. Number in meters.
    static int opaque_level = 128;  // default opaqueness if no transparency at all, range [0...255]

    wxString wks_ColourSettings;

    wks_ColourSettings.append(wxString(_T("nv           0  0  0  0\r\n")));
    wks_ColourSettings.append(
        wxString::Format(_T("%f %i %i %i %i\r\n"),
            m_pconf->colour.m_twoColoursDepth + nci,
            m_pconf->colour.m_twoColours[0].Red(),
            m_pconf->colour.m_twoColours[0].Green(),
            m_pconf->colour.m_twoColours[0].Blue(),
            m_pconf->colour.m_twoColours[0].Alpha())
    );
    wks_ColourSettings.append(
        wxString::Format(_T("%f %i %i %i %i\r\n"),
            m_pconf->colour.m_twoColoursDepth,
            m_pconf->colour.m_twoColours[1].Red(),
            m_pconf->colour.m_twoColours[1].Green(),
            m_pconf->colour.m_twoColours[1].Blue(),
            opaque_level)
    );

    return wks_ColourSettings;
}

