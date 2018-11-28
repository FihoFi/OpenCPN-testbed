/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  LIVI_Depth_model Plugin
 * Author:   Marjukka Kokkonen
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Mike Rossiter                                   *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include "LIVI_Depth_model_pi_UI_impl.h"
#include <wx/progdlg.h>
#include <wx/wx.h>
#include "wx/dir.h"
#include <list>
#include <cmath>
#include <iomanip> // setprecision
#include <sstream> // stringstream

#include "LIVI_Depth_model_pi.h"
#include "dm_API.h"

class Position;
class LIVI_Depth_model_pi;

#define FAIL(X) do { error = X; goto failed; } while(0)

Dlg::Dlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
    : LIVIDMUI_DLG( parent, id, title, pos, size, style )
{
    this->Fit();
}

void Dlg::OnClose(wxCloseEvent& event)
{   plugin->OnDepthModelDialogClose();   }

void Dlg::OnFileImportFileChange(wxFileDirPickerEvent& WXUNUSED(event))
{
    //wxFileName fname = this->dmPictureImport_filePicker->GetFileName();
    //plugin->OnFileImportFileChange(fname);
}

void Dlg::OnImageFileChange(wxFileDirPickerEvent& WXUNUSED(event))
{
    wxFileName fname = this->dmPictureImport_filePicker->GetFileName();
    plugin->OnImageFileChange(fname);
}

void Dlg::OnGenerateImage(wxCommandEvent& WXUNUSED(event))
{
    plugin->OnGenerateImage();
}

void Dlg::OnClearImage(wxCommandEvent& WXUNUSED(event))
{
    plugin->OnClearImage();
}

void Dlg::OnChartTypeChange(wxChoicebookEvent& WXUNUSED(event))
{
    plugin->OnChartTypeChange(GetSelectedChartOption());
}

void Dlg::OnColourSchemaChange(wxChoicebookEvent& WXUNUSED(event))
{
    plugin->OnColourSchemaChange(GetSelectedColourOption());
}

void Dlg::OnUserColourFileChange(wxFileDirPickerEvent& WXUNUSED(event))
{
    wxFileName fname = this->dmColourOptionsUserFile_filePicker->GetFileName();
    plugin->OnUserColourFileChange(fname);
}

void Dlg::OnCurrentWaterLevelChange(wxSpinDoubleEvent& WXUNUSED(event))
{
    double cwl = this->dmWaterLevel_CurrentWaterLevel_spinCtrlDouble->GetValue();
    plugin->OnCurrentWaterLevelChange(cwl);
}

void Dlg::OnVerticalReferenceSystemOffsetChange(wxSpinDoubleEvent& WXUNUSED(event))
{
    double vrso = this->dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble->GetValue();
    plugin->OnVerticalReferenceSystemOffsetChange(vrso);
}

void Dlg::SetAboutInfo()
{
    this->dmAbout_icon_bitmap->SetIcon(plugin->GetIcon());

    this->dmAbout_name_staticText->SetLabelText(plugin->GetCommonName());

    this->dmAbout_version_staticText->SetLabel(wxString::Format(wxT(" v. %i.%i"),
        plugin->GetPlugInVersionMajor(), plugin->GetPlugInVersionMinor()));
    this->dmAbout_description_staticText->SetLabel(plugin->GetLongDescription());
    this->dmAbout_copyright_staticText->SetLabel(plugin->GetCopyright());

    this->dmAbout_development_team_staticText->SetLabel(
        "Development team\n"
        "    Stefan Engström,   LIVI (project owner)\n"
        "    Tiina Ketolainen,  Sitowise (dev team coordinator)\n"
        "    Marjukka Kokkonen, Sitowise (front development)\n"
        "    Tommi Larjomaa,    Sitowise (backend development)\n"
        "    Jarkko Ryyppö,     Sitowise (coordinate specialist)\n");
    // aboutInfo.SetWebSite("http://notavailable.yet");
}

void Dlg::OnAboutWxWidgets(wxCommandEvent& WXUNUSED(event))
{
    wxInfoMessageBox(this);
}

void Dlg::SetSelectedChartOption(DM_visualization choiceId)
{
    int pageId = 0;  // color relief

    switch (choiceId)
    {
    case COLOR_RELIEF:          pageId = 0; break;
    case HILLSHADE:             pageId = 1; break;
    case NONE:                  pageId = 2; break;

    default:                    pageId = 0; break;  // color relief
    }

    this->dmChartOptions_choicebook->ChangeSelection(pageId);
}

DM_visualization Dlg::GetSelectedChartOption()
{
    wxChoice* choice = this->dmChartOptions_choicebook->GetChoiceCtrl();
    int acceptedSelection = choice->GetSelection();
    wxString chString = choice->GetString(acceptedSelection);

    if      (chString.Contains("illshade")) { return HILLSHADE;     }   // Hillshade
    else if (chString.Contains("lain"))     { return NONE;          }   // Plain image
    else if (chString.Contains("epth"))     { return COLOR_RELIEF;  }   // Depth model / color relief
    else if (chString.Contains("elief"))    { return COLOR_RELIEF;  }   // Depth model / color relief
    else                          { return VISUALIZATION_UNDEFINED; }   // No such option
}

void Dlg::SetSelectedColourOption(DM_colourType choiceId)
{
    int pageId = 1;  // Five ranges

    switch (choiceId)
    {
    case COLOUR_USER_FILE:          pageId = 0; break;
    case COLOUR_FIVE_RANGES:        pageId = 1; break;
    case COLOUR_SLIDING:            pageId = 2; break;
    case COLOUR_TWO_RANGES:         pageId = 3; break;

    default:                        pageId = 1; break;  // Five ranges
    }

    this->dmColourOptions_choisebook->ChangeSelection(pageId);
}

DM_colourType Dlg::GetSelectedColourOption()
{
    wxChoice* choice = this->dmColourOptions_choisebook->GetChoiceCtrl();
    int acceptedSelection = choice->GetSelection();
    wxString chString = choice->GetString(acceptedSelection);

    if      (chString.Contains("file"))     { return COLOUR_USER_FILE;   }   // Use user given file
    else if (chString.Contains("ive"))      { return COLOUR_FIVE_RANGES; }   // Five depth ranges
    else if (chString.Contains("liding"))   { return COLOUR_SLIDING;     }   // Sliding colouring
    else if (chString.Contains("Two"))      { return COLOUR_TWO_RANGES;  }   // Two depth ranges
    else                                    { return COLOUR_UNDEFINED;   }   // No such option
}

void Dlg::SetCustomColor(int num, wxColour &col) {

    switch (num) {
        case 0:    this->dmColorOptionsCustom_ColourPicker1->SetColour(col);    break;
        case 1:    this->dmColorOptionsCustom_ColourPicker2->SetColour(col);    break;
        case 2:    this->dmColorOptionsCustom_ColourPicker3->SetColour(col);    break;
        case 3:    this->dmColorOptionsCustom_ColourPicker4->SetColour(col);    break;
        case 4:    this->dmColorOptionsCustom_ColourPicker5->SetColour(col);    break;
        default:
            // there is no such Colour picker to be altered
            break;
    }
}

wxColour Dlg::GetCustomColor(int num) {
    wxColourPickerCtrl* picker;
    switch (num) {
    case 0:    picker = this->dmColorOptionsCustom_ColourPicker1;    break;
    case 1:    picker = this->dmColorOptionsCustom_ColourPicker2;    break;
    case 2:    picker = this->dmColorOptionsCustom_ColourPicker3;    break;
    case 3:    picker = this->dmColorOptionsCustom_ColourPicker4;    break;
    case 4:    picker = this->dmColorOptionsCustom_ColourPicker5;    break;
    default:
        // there is no such Colour picker to be retrieved
        return wxColour();  break;
    }
//    wxColour* col;
    return picker->GetColour();
}

void Dlg::SetUserColourConfigurationFileName(wxFileName &fileName)
{    this->dmColourOptionsUserFile_filePicker->SetFileName(fileName);    }

wxFileName Dlg::GetUserColourConfigurationFileName()
{    return this->dmColourOptionsUserFile_filePicker->GetFileName();     }

void Dlg::SetCustomLevel(int num, double level)
{
    switch (num) {
    case 0:    this->dmColorOptionsCustom_SpinCtrlDouble1->SetValue(level);    break;
    case 1:    this->dmColorOptionsCustom_SpinCtrlDouble2->SetValue(level);    break;
    case 2:    this->dmColorOptionsCustom_SpinCtrlDouble3->SetValue(level);    break;
    case 3:    this->dmColorOptionsCustom_SpinCtrlDouble4->SetValue(level);    break;
    default:
        // there is no such Colour picker to be altered
        break;
    }
}

double Dlg::GetCustomLevel(int num) {
    wxSpinCtrlDouble* spinCtrl;
    switch (num) {
    case 0:    spinCtrl = this->dmColorOptionsCustom_SpinCtrlDouble1;    break;
    case 1:    spinCtrl = this->dmColorOptionsCustom_SpinCtrlDouble2;    break;
    case 2:    spinCtrl = this->dmColorOptionsCustom_SpinCtrlDouble3;    break;
    case 3:    spinCtrl = this->dmColorOptionsCustom_SpinCtrlDouble4;    break;
    default:
        // there is no such Colour picker to be altered
        return -9999;  break;
    }
    return spinCtrl->GetValue();
}

void Dlg::SetTwoColours(int num, wxColour& col)
{
    switch (num) {
    case 0:    this->dmColourOptionsTwoColours_TooShallow_ColourPicker->SetColour(col);    break;
    case 1:    this->dmColourOptionsTwoColours_DeepEnough_ColourPicker->SetColour(col);    break;
    default:
        // there is no such Colour picker to be altered
        break;
    }
}

wxColour Dlg::GetTwoColours(int num)
{
    wxColourPickerCtrl* picker;
    switch (num) {
    case 0:    picker = this->dmColourOptionsTwoColours_TooShallow_ColourPicker;    break;
    case 1:    picker = this->dmColourOptionsTwoColours_DeepEnough_ColourPicker;    break;
    default:
        // there is no such Colour picker to be retrieved
        return wxColour();  break;
    }
    return picker->GetColour();
}

void Dlg::SetDividingLevel(double level)
{
    this->dmColourOptionsTwoColours_DividingDepth_spinCtrlDouble->SetValue(level);
}

double Dlg::GetDividingLevel()
{
    return this->dmColourOptionsTwoColours_DividingDepth_spinCtrlDouble->GetValue();
}

void Dlg::SetCurrentWaterLevel(double level)
{
    this->dmWaterLevel_CurrentWaterLevel_spinCtrlDouble->SetValue(level);
}

double Dlg::GetCurrentWaterLevel()
{
    wxSpinCtrlDouble* spinCtrl;
    spinCtrl = this->dmWaterLevel_CurrentWaterLevel_spinCtrlDouble;
    return spinCtrl->GetValue();
}

void Dlg::SetVerticalReferenceSystemOffsetLevel(double level)
{
    this->dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble->SetValue(level);
}

double Dlg::GetVerticalReferenceSystemOffsetLevel()
{
    return this->dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble->GetValue();
}

wxFileName Dlg::GetDepthChartFileName()
{    return this->dmPictureImport_filePicker->GetFileName();    }

void    Dlg::SetDepthChartFileName(wxFileName &fileName)
{    this->dmPictureImport_filePicker->SetFileName(fileName);   }

void Dlg::SetCurrentlyDrawnTextToNothing()
{
    std::stringstream stream;
    stream << "Currently drawn:\n  Nothing";

    this->dmPictureImport_CurrentlyDrawn_staticText->SetLabel(stream.str());
}

void Dlg::SetCurrentlyDrawnText(DM_visualization chartType, DM_colourType colourSchema,
                                double wl, double vrso)
{
    std::stringstream stream;
    stream  << "Currently drawn:\n  "
            << getDrawingOptionsString(chartType, colourSchema, wl, vrso);

    this->dmPictureImport_CurrentlyDrawn_staticText->SetLabel(wxString(stream.str()));
}

void Dlg::SetToGenerateText(DM_visualization chartType, DM_colourType colourSchema,
                            double wl, double vrso)
{
    std::stringstream stream;
    stream  << "Drawing options for image to generate:\n  "
            << getDrawingOptionsString(chartType, colourSchema, wl, vrso);

    this->dmPictureImport_ToGenerate_staticText->SetLabel(wxString(stream.str()));
}

std::string Dlg::getDrawingOptionsString(DM_visualization chartType,
                                         DM_colourType colourSchema,
                                         double wl, double vrso)
{
    std::stringstream stream;
    std::string str;

    stream  << std::fixed << std::setprecision(2)
            << chartTypeToString(chartType);

    bool canApplyWaterLevels = false;
    if (chartType == COLOR_RELIEF)
    {
        if (colourSchema == COLOUR_FIVE_RANGES || colourSchema == COLOUR_TWO_RANGES)
        {   canApplyWaterLevels = true;     }

        stream << " / " + colouringTypeToString(colourSchema);
    }

    if (canApplyWaterLevels)
        stream  << "\n  Water level:   " << (wl   > 0 ? "+" : "") << wl
                << "\n  System offset: " << (vrso > 0 ? "+" : "") << vrso;
    else
        stream  << "\n  Water level:   (cannot apply)"
                << "\n  System offset: (cannot apply)";

    return stream.str();
}

void Dlg::SetPictureImportInfoText(std::string infoStr)
{
    this->dmPictureImport_Status_staticText->SetLabel(infoStr);
    this->dmPictureImport_Status_staticText->SetForegroundColour(*wxBLACK);
    this->dmPictureImport_Status_staticText->Refresh();
}

void Dlg::SetPictureImportErrorText(std::string errStr)
{
    this->dmPictureImport_Status_staticText->SetLabel(errStr);
    this->dmPictureImport_Status_staticText->SetForegroundColour(*wxRED);
    this->dmPictureImport_Status_staticText->Refresh();
}

void Dlg::SetHillshadeparamsText(double azimuth, double altitude,
                                 double zFactor, unsigned int transparency)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2)
        << "\nCurrent hillshade parameters:"
        << "\n  Azimuth of the light (0-360 degrees): " << azimuth
        << "\n  Altitude of the light (0-90 degrees): " << altitude
        << "\n  Elevation exaggeration (\"z factor\"): " << zFactor
        << "\n  Image transparency (0(none)-255(full)): " << transparency
        << "\n"
        << "\n"
        << "\nChange these parameters in the opencpn.ini file"
        << "\n(locate the opencpn.log, you'll find the .ini, too)."
        << "\nNew values are taken into use at OpenCPN restart."
        << "\n";

    std::string hillshadeStr = stream.str();
    this->dmChartOptions_Hillshade_paramInfoText->SetLabel(hillshadeStr);
}

//void Dlg::Addpoint(TiXmlElement* Route, wxString ptlat, wxString ptlon, wxString ptname, wxString ptsym, wxString pttype){
/*
//add point
TiXmlElement * RoutePoint = new TiXmlElement( "rtept" );
RoutePoint->SetAttribute("lat", ptlat.mb_str());
RoutePoint->SetAttribute("lon", ptlon.mb_str());


TiXmlElement * Name = new TiXmlElement( "name" );
TiXmlText * text = new TiXmlText( ptname.mb_str() );
RoutePoint->LinkEndChild( Name );
Name->LinkEndChild( text );

TiXmlElement * Symbol = new TiXmlElement( "sym" );
TiXmlText * text1 = new TiXmlText( ptsym.mb_str() );
RoutePoint->LinkEndChild( Symbol );
Symbol->LinkEndChild( text1 );

TiXmlElement * Type = new TiXmlElement( "type" );
TiXmlText * text2 = new TiXmlText( pttype.mb_str() );
RoutePoint->LinkEndChild( Type );
Type->LinkEndChild( text2 );
Route->LinkEndChild( RoutePoint );
//done adding point
*/
//}

//void Dlg::OnPSGPX( wxCommandEvent& event ){   Calculate(event, true, 1);}

bool Dlg::OpenXML()
{
    /*
    Position my_position;

    my_positions.clear();

    int response = wxID_CANCEL;
    int my_count = 0;

    wxArrayString file_array;
    wxString filename;
    wxFileDialog openDialog( this, _( "Import GPX Route file" ), m_gpx_path, wxT ( "" ),
    wxT ( "GPX files (*.gpx)|*.gpx|All files (*.*)|*.*" ),
    wxFD_OPEN | wxFD_MULTIPLE );
    response = openDialog.ShowModal();
    if( response == wxID_OK ) {
    openDialog.GetPaths( file_array );

    //    Record the currently selected directory for later use
    if( file_array.GetCount() ) {
    wxFileName fn( file_array[0] );
    filename = file_array[0];
    m_gpx_path = fn.GetPath();
    }
    }
    else if(response = wxID_CANCEL){
    return false;
    }

    TiXmlDocument doc;
    wxString error;
    wxProgressDialog *progressdialog = NULL;


    if(!doc.LoadFile(filename.mb_str())){
    FAIL(_("Failed to load file: ") + filename);
    }
    else {
    TiXmlElement *root = doc.RootElement();
    if(!strcmp(root->Value(), "rte"))
    FAIL(_("rte Invalid xml file"));

    int count = 0;
    for(TiXmlElement* e = root->FirstChildElement(); e; e = e->NextSiblingElement())
    count++;

    int i=0;
    for(TiXmlElement* e = root->FirstChildElement(); e; e = e->NextSiblingElement(), i++) {
    if(progressdialog) {
    if(!progressdialog->Update(i))
    return true;
    } else {
    if(1) {
    progressdialog = new wxProgressDialog(
    _("Route"), _("Loading"), count, this,
    wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
    }
    }

    for(TiXmlElement* f = e->FirstChildElement(); f; f = f->NextSiblingElement()) {
    if(!strcmp(f->Value(), "rtept")) {
    wxString rte_lat = wxString::FromUTF8(f->Attribute("lat"));
    wxString rte_lon = wxString::FromUTF8(f->Attribute("lon"));

    my_position.lat = rte_lat;
    my_position.lon = rte_lon;
    my_positions.push_back(my_position);
    }  //else if(!strcmp(f->Value(), "extensions")) {
    //rte_start = wxString::FromUTF8(f->Attribute("opencpn:start"));
    //rte_end = wxString::FromUTF8(f->Attribute("opencpn:end"));

    //}
    }

    }
    }

    delete progressdialog;
    return true;

    failed:
    delete progressdialog;

    wxMessageDialog mdlg(this, error, _("LIVI_Depth_model"), wxOK | wxICON_ERROR);
    mdlg.ShowModal();
    */

    return false;
}

//void Dlg::Calculate( wxCommandEvent& event, bool write_file, int Pattern  ){
/*
if(OpenXML()){

bool error_occured=false;
// double dist, fwdAz, revAz;


double lat1,lon1;
// if(!this->m_Lat1->GetValue().ToDouble(&lat1)){ lat1=0.0;}
// if(!this->m_Lon1->GetValue().ToDouble(&lon1)){ lon1=0.0;}
int num_hours;

num_hours = this->m_Nship->GetSelection();

// wxString str_countPts =  wxString::Format(wxT("%d"), (int)num_hours);
// wxMessageBox(str_countPts,_T("count_hours"));

lat1 = 0.0;
lon1 = 0.0;
//if (error_occured) wxMessageBox(_T("error in conversion of input coordinates"));

//error_occured=false;
wxString s;
if (write_file){
wxFileDialog dlg(this, _("Export LIVI_Depth_model Positions in GPX file as"), wxEmptyString, wxEmptyString, _T("GPX files (*.gpx)|*.gpx|All files (*.*)|*.*"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
if (dlg.ShowModal() == wxID_CANCEL){
error_occured=true;     // the user changed idea...
return;
}

//dlg.ShowModal();
s=dlg.GetPath();
//  std::cout<<s<< std::endl;
if (dlg.GetPath() == wxEmptyString){ error_occured=true; if (dbg) printf("Empty Path\n");}
}

//Validate input ranges
if (!error_occured){
if (std::abs(lat1)>90){ error_occured=true; }
if (std::abs(lon1)>180){ error_occured=true; }
if (error_occured) wxMessageBox(_("error in input range validation"));
}

//Start GPX
TiXmlDocument doc;
TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
doc.LinkEndChild( decl );
TiXmlElement * root = new TiXmlElement( "gpx" );
TiXmlElement * Route = new TiXmlElement( "rte" );
TiXmlElement * RouteName = new TiXmlElement( "name" );
TiXmlText * text4 = new TiXmlText( this->m_Route->GetValue().ToUTF8() );

if (write_file){
doc.LinkEndChild( root );
root->SetAttribute("version", "0.1");
root->SetAttribute("creator", "LIVI_Depth_model_pi by Sitowise & Liikennevirasto");
root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
root->SetAttribute("xmlns:gpxx","http://www.garmin.com/xmlschemas/GpxExtensions/v3" );
root->SetAttribute("xsi:schemaLocation", "http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd");
root->SetAttribute("xmlns:opencpn","http://www.opencpn.org");
Route->LinkEndChild( RouteName );
RouteName->LinkEndChild( text4 );


TiXmlElement * Extensions = new TiXmlElement( "extensions" );

TiXmlElement * StartN = new TiXmlElement( "opencpn:start" );
TiXmlText * text5 = new TiXmlText( "Start" );
Extensions->LinkEndChild( StartN );
StartN->LinkEndChild( text5 );

TiXmlElement * EndN = new TiXmlElement( "opencpn:end" );
TiXmlText * text6 = new TiXmlText( "End" );
Extensions->LinkEndChild( EndN );
EndN->LinkEndChild( text6 );

Route->LinkEndChild( Extensions );
}

switch ( Pattern ) {
case 1:
{
if (dbg) cout<<"LIVI_Depth_model Calculation\n";
double speed=0;
int    interval=1;

if(!this->m_Speed_PS->GetValue().ToDouble(&speed)){ speed=5.0;} // 5 kts default speed
interval = m_Nship->GetCurrentSelection();//S=1

speed = speed*interval;

int n=0;
//int multiplier=1;
double lati, loni;
double latN[100], lonN[100];
double latF, lonF;

Position my_point;

double value, value1;

for(std::vector<Position>::iterator it = my_positions.begin();  it != my_positions.end(); it++){

//			if(!(*it).lat.ToDouble(&value)){ /* error! *//* }
lati = value;
//			if(!(*it).lon.ToDouble(&value1)){ /* error! *//* }
loni = value1;

latN[n] = lati;
lonN[n] = loni;

n++;//0,1,2,3
}

my_positions.clear();

n--;//n = 2,  0,1,2
int routepoints = n+1; //3


double myDist, myBrng, myDistForBrng;
int count_pts;
double remaining_dist, myLast, route_dist;
remaining_dist = 0;
route_dist= 0;
myLast = 0;
myDistForBrng =0;
double total_dist = 0;
int i,c;
bool skip = false;
bool inloop = false;
bool setF = false;

latF = latN[0];
lonF = lonN[0];

// Start of new logic
for (i=0; i<n; i++){	// n is number of routepoints

// save the routepoint
my_point.lat = wxString::Format(wxT("%f"),latN[i]);
my_point.lon = wxString::Format(wxT("%f"),lonN[i]);
my_point.routepoint = 1;
my_point.wpt_num =  wxString::Format(wxT("%d"),(int)i);
my_points.push_back(my_point);

if (i==0){ // First F is a routepoint
latF = latN[i];
lonF = lonN[i];
}

DistanceBearingMercator(latN[i + 1], lonN[i + 1],latF, lonF,  &myDist, &myBrng);

total_dist = total_dist + myDist;

if (total_dist > speed){
// LIVI_Depth_model point after route point
//
route_dist = total_dist - myDist;
remaining_dist = speed - route_dist;

DistanceBearingMercator( latN[i + 1], lonN[i + 1], latN[i], lonN[i],&myDist, &myBrng);
destLoxodrome(latN[i], lonN[i], myBrng, remaining_dist, &lati, &loni);

// Put in LIVI_Depth_model after many route points
my_point.lat = wxString::Format(wxT("%f"),lati);
my_point.lon = wxString::Format(wxT("%f"),loni);
my_point.routepoint = 0;
my_points.push_back(my_point);

latF = lati;
lonF = loni;

total_dist = 0;

//
//
DistanceBearingMercator(latN[i + 1], lonN[i + 1], latF, lonF, &myDistForBrng, &myBrng);

if (myDistForBrng > speed){

// put in the LIVI_Depth_model positions
//
count_pts = (int)floor(myDistForBrng/speed);
//
remaining_dist = myDistForBrng - (count_pts*speed);
DistanceBearingMercator(latN[i + 1], lonN[i + 1], latF, lonF, &myDistForBrng, &myBrng);

for (c = 1; c <= count_pts ; c++){
destLoxodrome(latF, lonF, myBrng, speed*c, &lati, &loni);
// print mid points
my_point.lat = wxString::Format(wxT("%f"),lati);
my_point.lon = wxString::Format(wxT("%f"),loni);
my_point.routepoint = 0;
my_points.push_back(my_point);
//	End of prints
}

latF = lati;
lonF = loni;

total_dist = 0;
//
//
// All the LIVI_Depth_model positions inserted
}

if (total_dist == 0){
DistanceBearingMercator(latN[i + 1], lonN[i + 1], latF, lonF, &myDistForBrng, &myBrng);
total_dist = myDistForBrng;
latF = latN[i+1];
lonF = lonN[i+1];
}

}
else{
//
latF = latN[i+1];
lonF = lonN[i+1];
//
//
//
//
}   //

}
// End of new logic
// print the last routepoint
my_point.lat = wxString::Format(wxT("%f"),latN[i]);
my_point.lon = wxString::Format(wxT("%f"),lonN[i]);
my_point.routepoint = 1;
my_point.wpt_num =  wxString::Format(wxT("%d"),(int)i);
my_points.push_back(my_point);
//


for(std::vector<Position>::iterator itOut = my_points.begin();  itOut != my_points.end(); itOut++){
//wxMessageBox((*it).lat, _T("*it.lat"));

double value, value1;
//		if(!(*itOut).lat.ToDouble(&value)){ /* error! *//* }
lati = value;
//		if(!(*itOut).lon.ToDouble(&value1)){ /* error! *//* }
loni = value1;

if ((*itOut).routepoint == 1){
if (write_file){Addpoint(Route, wxString::Format(wxT("%f"),lati), wxString::Format(wxT("%f"),loni), (*itOut).wpt_num ,_T("diamond"),_T("WPT"));}
}
else{
if ((*itOut).routepoint == 0){
if (write_file){Addpoint(Route, wxString::Format(wxT("%f"),lati), wxString::Format(wxT("%f"),loni), _T("LIVI_Depth_model") ,_T("square"),_T("WPT"));}
}
}

}

my_points.clear();
break;

}


default:
{            // Note the colon, not a semicolon
cout<<"Error, bad input, quitting\n";
break;
}
}

if (write_file){
root->LinkEndChild( Route );
// check if string ends with .gpx or .GPX
if (!wxFileExists(s)){
s = s + _T(".gpx");
}
wxCharBuffer buffer=s.ToUTF8();
if (dbg) std::cout<< buffer.data()<<std::endl;
doc.SaveFile( buffer.data() );}
//} //end of if no error occured

if (error_occured==true)  {
wxLogMessage(_("Error in calculation. Please check input!") );
wxMessageBox(_("Error in calculation. Please check input!") );
}
}
*/
//}