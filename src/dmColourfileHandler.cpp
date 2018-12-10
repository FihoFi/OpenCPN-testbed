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


void dmColourfileHandler::setChartExtremeValues(double min, double max)
{
    chartMin = min;
    chartMax = max;
}

bool dmColourfileHandler::GetConfFileOfType(DM_colourType colourOption, wxFileName& colorFile)
{
    bool success = GenerateConfFileOfType(colourOption); // Generate from current options in use
    if (!success)
    {
        return false;
    }

    switch (colourOption)
    {
    case COLOUR_USER_FILE:   {   colorFile = m_pconf->colour.userColourConfPath;
                                 /*return GetUsersColorConfFile();*/     break; }
    case COLOUR_FIVE_RANGES: {   colorFile = fiveColoursFileName;        break; }
    case COLOUR_SLIDING:     {   colorFile = slidingColoursFileName;     break; }
    case COLOUR_TWO_RANGES:  {   colorFile = twoColoursFileName;         break; }
    default:                 {   colorFile = wxFileName("");             break; }
    }
    return true;
}

bool dmColourfileHandler::GenerateConfFileOfType(DM_colourType colourOption)
{
    bool success = true;
    switch (colourOption)
    {
    case COLOUR_USER_FILE:      {                                            break; }
    case COLOUR_FIVE_RANGES:    { success &= GenerateFiveColorConfFile();    break; }
    case COLOUR_SLIDING:        { success &= GenerateSlidingColorConfFile(); break; }
    case COLOUR_TWO_RANGES:     { success &= GenerateTwoColorConfFile();     break; }
    default:                    { success = false;                           break; }
    }
    return success;
}

bool dmColourfileHandler::GenerateFiveColorConfFile()
{
    wxString confText = GetFiveColourDepthColourWks();
    return GenerateColorConfFile(fiveColoursFileName, _T("five_colour_set.txt"), confText);
}

bool dmColourfileHandler::GenerateSlidingColorConfFile()
{
    wxString confText = GetSlidingColourDepthColourWks();
    return GenerateColorConfFile(slidingColoursFileName, _T("sliding_colour_set.txt"), confText);
}

bool dmColourfileHandler::GenerateTwoColorConfFile()
{
    wxString confText = GetTwoColourDepthColourWks();
    return GenerateColorConfFile(twoColoursFileName, _T("two_colour_set.txt"), confText);
}

bool dmColourfileHandler::GenerateColorConfFile(
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

    unsigned char alpha0, alpha1;
    double depth;
    for (int i = 0; i < DM_NUM_CUSTOM_DEP; i++) {
        alpha0 = m_pconf->colour.m_customColours[ i ].Alpha();
        alpha1 = m_pconf->colour.m_customColours[i+1].Alpha();

        depth =  m_pconf->colour.m_customDepths[i] +
                 m_pconf->waterLevel.m_currentWaterLevel+
                 m_pconf->waterLevel.m_verticalReferenceSystemOffset;

        wks_ColourSettings.append(
            wxString::Format(_T("%f %i %i %i %i\r\n"),
                depth + nci,
                m_pconf->colour.m_customColours[i].Red(),
                m_pconf->colour.m_customColours[i].Green(),
                m_pconf->colour.m_customColours[i].Blue(),
                (int)(alpha0 == wxALPHA_OPAQUE ? opaque_list[i] : alpha0))
        );

        wks_ColourSettings.append(
            wxString::Format(_T("%f %i %i %i %i\r\n"),
                depth,
                m_pconf->colour.m_customColours[i + 1].Red(),
                m_pconf->colour.m_customColours[i + 1].Green(),
                m_pconf->colour.m_customColours[i + 1].Blue(),
                (int)(alpha1 == wxALPHA_OPAQUE ? opaque_list[i+1] : alpha1))
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
    unsigned char alpha0 = m_pconf->colour.m_twoColours[0].Alpha();
    unsigned char alpha1 = m_pconf->colour.m_twoColours[1].Alpha();
    double depth = m_pconf->colour.m_twoColoursDepth +
                   m_pconf->waterLevel.m_currentWaterLevel +
                   m_pconf->waterLevel.m_verticalReferenceSystemOffset;

    wxString wks_ColourSettings;

    wks_ColourSettings.append(wxString(_T("nv           0  0  0  0\r\n")));

    wks_ColourSettings.append(
        wxString::Format(_T("%f %i %i %i %i\r\n"),
            depth + nci,
            m_pconf->colour.m_twoColours[0].Red(),
            m_pconf->colour.m_twoColours[0].Green(),
            m_pconf->colour.m_twoColours[0].Blue(),
            (int) (alpha0==wxALPHA_OPAQUE ? opaque_level : alpha0))
    );

    wks_ColourSettings.append(
        wxString::Format(_T("%f %i %i %i %i\r\n"),
            depth,
            m_pconf->colour.m_twoColours[1].Red(),
            m_pconf->colour.m_twoColours[1].Green(),
            m_pconf->colour.m_twoColours[1].Blue(),
            (int) (alpha1 == wxALPHA_OPAQUE ? opaque_level : alpha1))
    );

    return wks_ColourSettings;
}

