///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul 11 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "LIVI_Depth_model_pi_UI.h"

///////////////////////////////////////////////////////////////////////////

LIVIDMUI_DLG::LIVIDMUI_DLG( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* LIVIDMUISizer;
	LIVIDMUISizer = new wxBoxSizer( wxVERTICAL );
	
	dmTabChooser = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	dmFileImport_Panel = new wxPanel( dmTabChooser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("FIle input") );
	wxBoxSizer* dmFileImport_bSizer;
	dmFileImport_bSizer = new wxBoxSizer( wxVERTICAL );
	
	dmDepthModelFile_staticText = new wxStaticText( dmFileImport_Panel, wxID_ANY, wxT("Depth model file"), wxDefaultPosition, wxDefaultSize, 0 );
	dmDepthModelFile_staticText->Wrap( -1 );
	dmFileImport_bSizer->Add( dmDepthModelFile_staticText, 0, wxALL, 5 );
	
	dmPictureImport_filePicker = new wxFilePickerCtrl( dmFileImport_Panel, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( 380,-1 ), wxFLP_DEFAULT_STYLE );
	dmFileImport_bSizer->Add( dmPictureImport_filePicker, 0, wxALL, 5 );
	
	dmPictureImportError_staticText = new wxStaticText( dmFileImport_Panel, wxID_ANY, wxT("(everything ok)"), wxDefaultPosition, wxDefaultSize, 0 );
	dmPictureImportError_staticText->Wrap( -1 );
	dmFileImport_bSizer->Add( dmPictureImportError_staticText, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmFileImport_bSizer->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	
	dmFileImport_Panel->SetSizer( dmFileImport_bSizer );
	dmFileImport_Panel->Layout();
	dmFileImport_bSizer->Fit( dmFileImport_Panel );
	dmTabChooser->AddPage( dmFileImport_Panel, wxT("File import"), false );
	dmColorOptions_Panel = new wxPanel( dmTabChooser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmColorOptions_Panel->SetToolTip( wxT("Pick how you want the depth modol colors to be rendered") );
	
	wxBoxSizer* dmColorOptions_Sizer;
	dmColorOptions_Sizer = new wxBoxSizer( wxVERTICAL );
	
	dmColorOptions_SelectShema_Label = new wxStaticText( dmColorOptions_Panel, wxID_ANY, wxT("Select the color schema you want to use for the depth data"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptions_SelectShema_Label->Wrap( -1 );
	dmColorOptions_Sizer->Add( dmColorOptions_SelectShema_Label, 0, wxALL, 5 );
	
	dmColorOptions_Choisebook = new wxChoicebook( dmColorOptions_Panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	dmColorOptionsCustom_Panel = new wxPanel( dmColorOptions_Choisebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmColorOptionsCustom_Sizer;
	dmColorOptionsCustom_Sizer = new wxBoxSizer( wxVERTICAL );
	
	dmColorOptionsCustom_Label = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxT("Set colors, and their restricting depths"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_Label->Wrap( -1 );
	dmColorOptionsCustom_Sizer->Add( dmColorOptionsCustom_Label, 0, wxALL, 5 );
	
	wxFlexGridSizer* dmColorOptionsCustom_GridSizer;
	dmColorOptionsCustom_GridSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmColorOptionsCustom_GridSizer->SetFlexibleDirection( wxBOTH );
	dmColorOptionsCustom_GridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmColorOptionsCustom_TooShallow_Label = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxT("Too shallow, no go"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_TooShallow_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_TooShallow_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker1 = new wxColourPickerCtrl( dmColorOptionsCustom_Panel, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_ColourPicker1->SetMinSize( wxSize( 70,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker1, 0, wxALL, 5 );
	
	dmEmpty_Label1 = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label1->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label1, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble1 = new wxSpinCtrlDouble( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 0, -17.500000, 1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble1, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ABitRisky_Label = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxT("At high water only"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_ABitRisky_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ABitRisky_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker2 = new wxColourPickerCtrl( dmColorOptionsCustom_Panel, wxID_ANY, wxColour( 255, 196, 228 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_ColourPicker2->SetMinSize( wxSize( 70,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker2, 0, wxALL, 5 );
	
	dmEmpty_Label2 = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label2->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label2, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble2 = new wxSpinCtrlDouble( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 0, -20.000000, 1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble2, 0, wxALL, 5 );
	
	dmColorOptionsCustom_OK_Label = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_OK_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_OK_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker3 = new wxColourPickerCtrl( dmColorOptionsCustom_Panel, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_ColourPicker3->SetMinSize( wxSize( 70,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker3, 0, wxALL, 5 );
	
	dmEmpty_Label3 = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label3->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label3, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble3 = new wxSpinCtrlDouble( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 0, -22.500000, 1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble3, 0, wxALL, 5 );
	
	dmColorOptionsCustom_OKOnLowWater_Label = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxT("OK at low water"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_OKOnLowWater_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_OKOnLowWater_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker4 = new wxColourPickerCtrl( dmColorOptionsCustom_Panel, wxID_ANY, wxColour( 128, 196, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_ColourPicker4->SetMinSize( wxSize( 70,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker4, 0, wxALL, 5 );
	
	dmEmpty_Label4 = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label4->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label4, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble4 = new wxSpinCtrlDouble( dmColorOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 0, -25.000000, 1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble4, 0, wxALL, 5 );
	
	dmColorOptionsCustom_Deep_Label = new wxStaticText( dmColorOptionsCustom_Panel, wxID_ANY, wxT("Deep, good to go"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_Deep_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_Deep_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker5 = new wxColourPickerCtrl( dmColorOptionsCustom_Panel, wxID_ANY, wxColour( 0, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_ColourPicker5->SetMinSize( wxSize( 70,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker5, 0, wxALL, 5 );
	
	
	dmColorOptionsCustom_Sizer->Add( dmColorOptionsCustom_GridSizer, 1, wxEXPAND, 5 );
	
	
	dmColorOptionsCustom_Panel->SetSizer( dmColorOptionsCustom_Sizer );
	dmColorOptionsCustom_Panel->Layout();
	dmColorOptionsCustom_Sizer->Fit( dmColorOptionsCustom_Panel );
	dmColorOptions_Choisebook->AddPage( dmColorOptionsCustom_Panel, wxT("Custom color"), true );
	dmColorOptionsSliding_Panel = new wxPanel( dmColorOptions_Choisebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmColorOptionsSliding_Panel->Enable( false );
	
	wxFlexGridSizer* dmColorOptionsSliding_GridSizer;
	dmColorOptionsSliding_GridSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmColorOptionsSliding_GridSizer->SetFlexibleDirection( wxBOTH );
	dmColorOptionsSliding_GridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmColorOptionsSliding_DeepestLevelOfInterest_Label = new wxStaticText( dmColorOptionsSliding_Panel, wxID_ANY, wxT("Deepest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsSliding_DeepestLevelOfInterest_Label->Wrap( -1 );
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_DeepestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColorOptionsSliding_DeepestLevelOfInterest_TextBox = new wxTextCtrl( dmColorOptionsSliding_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsSliding_DeepestLevelOfInterest_TextBox->SetToolTip( wxT("Set deepest level of interest in [m]") );
	
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_DeepestLevelOfInterest_TextBox, 0, wxALL, 5 );
	
	dmColorOptionsSliding_ColorAtDeepestLevelOfInterest_Label = new wxStaticText( dmColorOptionsSliding_Panel, wxID_ANY, wxT("Color at deepest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsSliding_ColorAtDeepestLevelOfInterest_Label->Wrap( -1 );
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_ColorAtDeepestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColorOptionsSliding_ColorAtDeepestLevelOfInterest_ColorPicker = new wxColourPickerCtrl( dmColorOptionsSliding_Panel, wxID_ANY, wxColour( 0, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_ColorAtDeepestLevelOfInterest_ColorPicker, 0, wxALL, 5 );
	
	dmColorOptionsSliding_ShallowestLevelOfInterest_Label = new wxStaticText( dmColorOptionsSliding_Panel, wxID_ANY, wxT("Shallowest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsSliding_ShallowestLevelOfInterest_Label->Wrap( -1 );
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_ShallowestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_TextBox = new wxTextCtrl( dmColorOptionsSliding_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_TextBox->SetToolTip( wxT("Set shallowest level of interest in [m]") );
	
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_TextBox, 0, wxALL, 5 );
	
	dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_Label = new wxStaticText( dmColorOptionsSliding_Panel, wxID_ANY, wxT("Color at shallowest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_Label->Wrap( -1 );
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColorOptionsSliding_ShallowestLevelOfInterest_ColourPicker = new wxColourPickerCtrl( dmColorOptionsSliding_Panel, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsSliding_GridSizer->Add( dmColorOptionsSliding_ShallowestLevelOfInterest_ColourPicker, 0, wxALL, 5 );
	
	
	dmColorOptionsSliding_Panel->SetSizer( dmColorOptionsSliding_GridSizer );
	dmColorOptionsSliding_Panel->Layout();
	dmColorOptionsSliding_GridSizer->Fit( dmColorOptionsSliding_Panel );
	dmColorOptions_Choisebook->AddPage( dmColorOptionsSliding_Panel, wxT("Sliding color"), false );
	dmColorOptionsConstant_Panel = new wxPanel( dmColorOptions_Choisebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmColorOptionsConstant_Panel->Enable( false );
	
	wxFlexGridSizer* dmColorOptionsConstant_GridSizer;
	dmColorOptionsConstant_GridSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmColorOptionsConstant_GridSizer->SetFlexibleDirection( wxBOTH );
	dmColorOptionsConstant_GridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmColorOptionsConstant_SetStep_Label = new wxStaticText( dmColorOptionsConstant_Panel, wxID_ANY, wxT("Level difference between color change"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsConstant_SetStep_Label->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmColorOptionsConstant_SetStep_Label, 0, wxALL, 5 );
	
	dmColorOptionsConstant_SetStep_TextCtrl = new wxTextCtrl( dmColorOptionsConstant_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsConstant_SetStep_TextCtrl->SetToolTip( wxT("Set the step required depth change\nbetween color changes") );
	
	dmColorOptionsConstant_GridSizer->Add( dmColorOptionsConstant_SetStep_TextCtrl, 0, wxALL, 5 );
	
	dmColorOptionsConstant_ColorAtDeepestLevelOfInterest_Label1 = new wxStaticText( dmColorOptionsConstant_Panel, wxID_ANY, wxT("Color at deepest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsConstant_ColorAtDeepestLevelOfInterest_Label1->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmColorOptionsConstant_ColorAtDeepestLevelOfInterest_Label1, 0, wxALL, 5 );
	
	dmColorOptionsConstant_ColorAtDeepestLevelOfInterest_ColourPicker = new wxColourPickerCtrl( dmColorOptionsConstant_Panel, wxID_ANY, wxColour( 0, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsConstant_GridSizer->Add( dmColorOptionsConstant_ColorAtDeepestLevelOfInterest_ColourPicker, 0, wxALL, 5 );
	
	dmColorOptionsConstant_ColorAtShallowestLevelOfInterest_Label1 = new wxStaticText( dmColorOptionsConstant_Panel, wxID_ANY, wxT("Color at shallowest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsConstant_ColorAtShallowestLevelOfInterest_Label1->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmColorOptionsConstant_ColorAtShallowestLevelOfInterest_Label1, 0, wxALL, 5 );
	
	dmColorOptionsConstant_ShallowestLevelOfInterest_ColourPicker = new wxColourPickerCtrl( dmColorOptionsConstant_Panel, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsConstant_GridSizer->Add( dmColorOptionsConstant_ShallowestLevelOfInterest_ColourPicker, 0, wxALL, 5 );
	
	
	dmColorOptionsConstant_Panel->SetSizer( dmColorOptionsConstant_GridSizer );
	dmColorOptionsConstant_Panel->Layout();
	dmColorOptionsConstant_GridSizer->Fit( dmColorOptionsConstant_Panel );
	dmColorOptions_Choisebook->AddPage( dmColorOptionsConstant_Panel, wxT("Color changes at constant level steps"), false );
	dmColorOptionsUserFile_Panel = new wxPanel( dmColorOptions_Choisebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmColorOptionsUserFile_bSizer;
	dmColorOptionsUserFile_bSizer = new wxBoxSizer( wxVERTICAL );
	
	dmColorOptionsUserFile_giveFile_staticText = new wxStaticText( dmColorOptionsUserFile_Panel, wxID_ANY, wxT("Define your colour definition file to be used for the depth charts"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsUserFile_giveFile_staticText->Wrap( -1 );
	dmColorOptionsUserFile_bSizer->Add( dmColorOptionsUserFile_giveFile_staticText, 0, wxALL, 5 );
	
	dmColorOptionsUserFile_filePicker = new wxFilePickerCtrl( dmColorOptionsUserFile_Panel, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( 380,-1 ), wxFLP_DEFAULT_STYLE );
	dmColorOptionsUserFile_bSizer->Add( dmColorOptionsUserFile_filePicker, 0, wxALL, 5 );
	
	dmColorOptionsUserFile_help_staticText = new wxStaticText( dmColorOptionsUserFile_Panel, wxID_ANY, wxT("Coloring is made by GDAL/gdaldem.\nThe file must be a text file, containing following data at each row:\n\n<depth/percentage value> <r> <g> <b> <alpha>\n\nExample (depth values)         Example (percentage values)\n\n-10  255   0     0   128             10% 255   0      0  128    \n-15   0      0   255 128             21%   0     0   255 128    \n-20   0   255    0   128             75%   0   255   0   128    \nnv    0      0     0      0              nv      0     0     0     0     \n\nThe alpha value tells the amount of opaqueness (inverse of \ntransparency) for the colour.\n\"nv\" stands for \"no value\".\nAll of the values r,g,b, and alpha must be between 0...255."), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsUserFile_help_staticText->Wrap( -1 );
	dmColorOptionsUserFile_bSizer->Add( dmColorOptionsUserFile_help_staticText, 0, wxALL, 5 );
	
	
	dmColorOptionsUserFile_Panel->SetSizer( dmColorOptionsUserFile_bSizer );
	dmColorOptionsUserFile_Panel->Layout();
	dmColorOptionsUserFile_bSizer->Fit( dmColorOptionsUserFile_Panel );
	dmColorOptions_Choisebook->AddPage( dmColorOptionsUserFile_Panel, wxT("User given coloring file"), false );
	dmColorOptions_Sizer->Add( dmColorOptions_Choisebook, 1, wxEXPAND | wxALL, 5 );
	
	dmColorOptions_Apply_Button = new wxButton( dmColorOptions_Panel, wxID_ANY, wxT("Apply color settings"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptions_Sizer->Add( dmColorOptions_Apply_Button, 0, wxALL, 5 );
	
	
	dmColorOptions_Panel->SetSizer( dmColorOptions_Sizer );
	dmColorOptions_Panel->Layout();
	dmColorOptions_Sizer->Fit( dmColorOptions_Panel );
	dmTabChooser->AddPage( dmColorOptions_Panel, wxT("Depth color schemas"), true );
	dmWaterLevel_Panel = new wxPanel( dmTabChooser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmWaterLevel_Panel->Enable( false );
	dmWaterLevel_Panel->Hide();
	
	wxGridSizer* gSizer7;
	gSizer7 = new wxGridSizer( 0, 2, 0, 0 );
	
	dmEmpty = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty->Wrap( -1 );
	gSizer7->Add( dmEmpty, 0, wxALL, 5 );
	
	dmEmpty1 = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty1->Wrap( -1 );
	gSizer7->Add( dmEmpty1, 0, wxALL, 5 );
	
	dmWaterLevel_Label = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxT("Set current water level\nabove or below normal sea level"), wxDefaultPosition, wxDefaultSize, 0 );
	dmWaterLevel_Label->Wrap( -1 );
	gSizer7->Add( dmWaterLevel_Label, 0, wxALL, 5 );
	
	dmWaterLevel_textCtrl = new wxTextCtrl( dmWaterLevel_Panel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer7->Add( dmWaterLevel_textCtrl, 0, wxALL, 5 );
	
	
	dmWaterLevel_Panel->SetSizer( gSizer7 );
	dmWaterLevel_Panel->Layout();
	gSizer7->Fit( dmWaterLevel_Panel );
	dmTabChooser->AddPage( dmWaterLevel_Panel, wxT("Water level"), false );
	dmAbout_Panel = new wxPanel( dmTabChooser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmAbout_Sizer;
	dmAbout_Sizer = new wxBoxSizer( wxHORIZONTAL );
	
	dmAbout_Icon_Panel = new wxPanel( dmAbout_Panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmAbout_Icon_Panel->SetMaxSize( wxSize( 40,-1 ) );
	
	wxBoxSizer* dmAbout_Icon_Sizer;
	dmAbout_Icon_Sizer = new wxBoxSizer( wxVERTICAL );
	
	
	dmAbout_Icon_Panel->SetSizer( dmAbout_Icon_Sizer );
	dmAbout_Icon_Panel->Layout();
	dmAbout_Icon_Sizer->Fit( dmAbout_Icon_Panel );
	dmAbout_Sizer->Add( dmAbout_Icon_Panel, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* dmAbout_Sizer2;
	dmAbout_Sizer2 = new wxBoxSizer( wxVERTICAL );
	
	dmAbout_Label = new wxStaticText( dmAbout_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	dmAbout_Label->Wrap( -1 );
	dmAbout_Sizer2->Add( dmAbout_Label, 0, wxALL, 5 );
	
	dmAbout_LIVIDMPlugin_Button = new wxButton( dmAbout_Panel, wxID_ANY, wxT("About LIVI Depth model plugin"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	
	dmAbout_LIVIDMPlugin_Button->SetBitmapPosition( wxRIGHT );
	dmAbout_Sizer2->Add( dmAbout_LIVIDMPlugin_Button, 0, wxALL, 5 );
	
	dm_AboutWxWidgets_Button = new wxButton( dmAbout_Panel, wxID_ANY, wxT("About wxWidgets library used"), wxDefaultPosition, wxDefaultSize, 0 );
	dmAbout_Sizer2->Add( dm_AboutWxWidgets_Button, 0, wxALL, 5 );
	
	
	dmAbout_Sizer->Add( dmAbout_Sizer2, 1, wxEXPAND, 5 );
	
	
	dmAbout_Panel->SetSizer( dmAbout_Sizer );
	dmAbout_Panel->Layout();
	dmAbout_Sizer->Fit( dmAbout_Panel );
	dmTabChooser->AddPage( dmAbout_Panel, wxT("About"), false );
	
	LIVIDMUISizer->Add( dmTabChooser, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( LIVIDMUISizer );
	this->Layout();
	LIVIDMUISizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	dmPictureImport_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LIVIDMUI_DLG::OnFileImportFileChange ), NULL, this );
	dmColorOptions_Apply_Button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnColorOptionsApplyButtonClick ), NULL, this );
	dmAbout_LIVIDMPlugin_Button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnAboutLIVIDepthModel ), NULL, this );
	dm_AboutWxWidgets_Button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnAboutWxWidgets ), NULL, this );
}

LIVIDMUI_DLG::~LIVIDMUI_DLG()
{
	// Disconnect Events
	dmPictureImport_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LIVIDMUI_DLG::OnFileImportFileChange ), NULL, this );
	dmColorOptions_Apply_Button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnColorOptionsApplyButtonClick ), NULL, this );
	dmAbout_LIVIDMPlugin_Button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnAboutLIVIDepthModel ), NULL, this );
	dm_AboutWxWidgets_Button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnAboutWxWidgets ), NULL, this );
	
}
