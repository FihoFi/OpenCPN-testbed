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
	
	dmPictureImport_filePicker = new wxFilePickerCtrl( dmFileImport_Panel, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("Tif/Tiff  image file(*.tif;*.tiff)|*.tif;*.tiff|BAG file (*.bag)|*.bag|PNG (*.png)|*.png|Any file (*.*)|*.*"), wxDefaultPosition, wxSize( 380,-1 ), wxFLP_DEFAULT_STYLE );
	dmFileImport_bSizer->Add( dmPictureImport_filePicker, 0, wxALL, 5 );
	
	dmPictureImport_CurrentlyDrawn_staticText = new wxStaticText( dmFileImport_Panel, wxID_ANY, wxT("Currently drawn:\n  Nothing"), wxDefaultPosition, wxDefaultSize, 0 );
	dmPictureImport_CurrentlyDrawn_staticText->Wrap( -1 );
	dmPictureImport_CurrentlyDrawn_staticText->SetMinSize( wxSize( -1,80 ) );
	
	dmFileImport_bSizer->Add( dmPictureImport_CurrentlyDrawn_staticText, 0, wxALL, 5 );
	
	dmPictureImport_Status_staticText = new wxStaticText( dmFileImport_Panel, wxID_ANY, wxT("Plugin initialized"), wxDefaultPosition, wxDefaultSize, 0|wxBORDER_THEME );
	dmPictureImport_Status_staticText->Wrap( -1 );
	dmPictureImport_Status_staticText->SetMinSize( wxSize( 350,50 ) );
	
	dmFileImport_bSizer->Add( dmPictureImport_Status_staticText, 0, wxALL, 5 );
	
	wxFlexGridSizer* dmPictureImport_fgSizer;
	dmPictureImport_fgSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmPictureImport_fgSizer->SetFlexibleDirection( wxBOTH );
	dmPictureImport_fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmPictureImport_ToGenerate_staticText = new wxStaticText( dmFileImport_Panel, wxID_ANY, wxT("Drawing options for image to generate:\n  Colour relief image\n  Five depth ranges\n  Water level: \n  System offset: "), wxDefaultPosition, wxDefaultSize, 0 );
	dmPictureImport_ToGenerate_staticText->Wrap( -1 );
	dmPictureImport_ToGenerate_staticText->SetMinSize( wxSize( -1,80 ) );
	
	dmPictureImport_fgSizer->Add( dmPictureImport_ToGenerate_staticText, 0, wxALL, 5 );
	
	wxWrapSizer* dmPictureImport_buttons_wSizer;
	dmPictureImport_buttons_wSizer = new wxWrapSizer( wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS );
	
	dmPictureImport_buttons_wSizer->SetMinSize( wxSize( 215,-1 ) ); 
	dmPictureImport_GenerateImage_button = new wxButton( dmFileImport_Panel, wxID_ANY, wxT("Generate image"), wxDefaultPosition, wxDefaultSize, 0 );
	dmPictureImport_buttons_wSizer->Add( dmPictureImport_GenerateImage_button, 0, wxALL, 5 );
	
	dmPictureImport_ClearImage_button = new wxButton( dmFileImport_Panel, wxID_ANY, wxT("Clear image"), wxDefaultPosition, wxDefaultSize, 0 );
	dmPictureImport_buttons_wSizer->Add( dmPictureImport_ClearImage_button, 0, wxALL, 5 );
	
	
	dmPictureImport_fgSizer->Add( dmPictureImport_buttons_wSizer, 1, wxEXPAND, 5 );
	
	
	dmFileImport_bSizer->Add( dmPictureImport_fgSizer, 1, wxEXPAND, 5 );
	
	
	dmFileImport_Panel->SetSizer( dmFileImport_bSizer );
	dmFileImport_Panel->Layout();
	dmFileImport_bSizer->Fit( dmFileImport_Panel );
	dmTabChooser->AddPage( dmFileImport_Panel, wxT("File import"), false );
	dmChartOptions_Panel = new wxPanel( dmTabChooser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmChartOptions_Panel->SetToolTip( wxT("Pick how you want the depth modol colors to be rendered") );
	
	wxBoxSizer* dmChartOptions_Sizer;
	dmChartOptions_Sizer = new wxBoxSizer( wxVERTICAL );
	
	dmChartOptions_staticText = new wxStaticText( dmChartOptions_Panel, wxID_ANY, wxT("Pick the way you want the chart to be rendered"), wxDefaultPosition, wxDefaultSize, 0 );
	dmChartOptions_staticText->Wrap( -1 );
	dmChartOptions_Sizer->Add( dmChartOptions_staticText, 0, wxALL, 5 );
	
	dmChartOptions_choicebook = new wxChoicebook( dmChartOptions_Panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	dmChartOptions_ColorRelief_panel = new wxPanel( dmChartOptions_choicebook, DM_viz_COLOR_RELIEF, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmChartOptions_ColorRelief_Sizer;
	dmChartOptions_ColorRelief_Sizer = new wxBoxSizer( wxVERTICAL );
	
	dmChartOptions_ColourRelief_staticText = new wxStaticText( dmChartOptions_ColorRelief_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmChartOptions_ColourRelief_staticText->Wrap( -1 );
	dmChartOptions_ColorRelief_Sizer->Add( dmChartOptions_ColourRelief_staticText, 0, wxALL, 5 );
	
	dmColourOptions_SelectShema_Label = new wxStaticText( dmChartOptions_ColorRelief_panel, wxID_ANY, wxT("Color schema for the depth colouring"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptions_SelectShema_Label->Wrap( -1 );
	dmChartOptions_ColorRelief_Sizer->Add( dmColourOptions_SelectShema_Label, 0, wxALL, 5 );
	
	dmColourOptions_choisebook = new wxChoicebook( dmChartOptions_ColorRelief_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	dmColourOptionsUserFile_Panel = new wxPanel( dmColourOptions_choisebook, DM_viz_USER_FILE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmColourOptionsUserFile_bSizer;
	dmColourOptionsUserFile_bSizer = new wxBoxSizer( wxVERTICAL );
	
	dmEmpty_staticText1 = new wxStaticText( dmColourOptionsUserFile_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_staticText1->Wrap( -1 );
	dmColourOptionsUserFile_bSizer->Add( dmEmpty_staticText1, 0, wxALL, 5 );
	
	dmColourOptionsUserFile_giveFile_staticText = new wxStaticText( dmColourOptionsUserFile_Panel, wxID_ANY, wxT("Your colour definition file:"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsUserFile_giveFile_staticText->Wrap( -1 );
	dmColourOptionsUserFile_bSizer->Add( dmColourOptionsUserFile_giveFile_staticText, 0, wxALL, 5 );
	
	dmColourOptionsUserFile_filePicker = new wxFilePickerCtrl( dmColourOptionsUserFile_Panel, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("Text file(*.txt)|*.txt|Any file (*.*)|*.*"), wxDefaultPosition, wxSize( 360,-1 ), wxFLP_DEFAULT_STYLE );
	dmColourOptionsUserFile_bSizer->Add( dmColourOptionsUserFile_filePicker, 0, wxALL, 5 );
	
	dmColourOptionsUserFile_help_staticText = new wxStaticText( dmColourOptionsUserFile_Panel, wxID_ANY, wxT("Colouring is made by GDAL/gdaldem.\nThe file must be a text file, containing following data at each row:\n\n<depth/percentage value> <r> <g> <b> <alpha>\n\nExample (depth values)         Example (percentage values)\n\n-10  255   0     0   128             10% 255   0      0  128    \n-15   0      0   255 128             21%   0     0   255 128    \n-20   0   255    0   128             75%   0   255   0   128    \nnv    0      0     0      0              nv      0     0     0     0     \n\nThe alpha value tells the amount of opaqueness (inverse of \ntransparency) for the colour.\n\"nv\" stands for \"no value\".\nAll of the values r,g,b, and alpha must be between 0...255."), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsUserFile_help_staticText->Wrap( -1 );
	dmColourOptionsUserFile_help_staticText->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	
	dmColourOptionsUserFile_bSizer->Add( dmColourOptionsUserFile_help_staticText, 0, wxALL, 5 );
	
	
	dmColourOptionsUserFile_Panel->SetSizer( dmColourOptionsUserFile_bSizer );
	dmColourOptionsUserFile_Panel->Layout();
	dmColourOptionsUserFile_bSizer->Fit( dmColourOptionsUserFile_Panel );
	dmColourOptions_choisebook->AddPage( dmColourOptionsUserFile_Panel, wxT("Give your colour definition file"), false );
	dmColourOptionsCustom_Panel = new wxPanel( dmColourOptions_choisebook, DM_viz_FIVE_DEPTH_RANGES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmColorOptionsCustom_Sizer;
	dmColorOptionsCustom_Sizer = new wxBoxSizer( wxVERTICAL );
	
	dmEmpty_staticText2 = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_staticText2->Wrap( -1 );
	dmColorOptionsCustom_Sizer->Add( dmEmpty_staticText2, 0, wxALL, 5 );
	
	dmColorOptionsCustom_Label = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxT("Set colors, and their restricting depths"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_Label->Wrap( -1 );
	dmColorOptionsCustom_Sizer->Add( dmColorOptionsCustom_Label, 0, wxALL, 5 );
	
	wxFlexGridSizer* dmColorOptionsCustom_GridSizer;
	dmColorOptionsCustom_GridSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmColorOptionsCustom_GridSizer->SetFlexibleDirection( wxBOTH );
	dmColorOptionsCustom_GridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmColorOptionsCustom_TooShallow_Label = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxT("Too shallow, no go"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_TooShallow_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_TooShallow_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker1 = new wxColourPickerCtrl( dmColourOptionsCustom_Panel, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker1, 0, wxALL, 5 );
	
	dmEmpty_Label1 = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label1->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label1, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble1 = new wxSpinCtrlDouble( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 0, -17.500000, 1 );
	dmColorOptionsCustom_SpinCtrlDouble1->SetMaxSize( wxSize( 100,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble1, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ABitRisky_Label = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxT("Cautiously on high water"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_ABitRisky_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ABitRisky_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker2 = new wxColourPickerCtrl( dmColourOptionsCustom_Panel, wxID_ANY, wxColour( 255, 196, 228 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker2, 0, wxALL, 5 );
	
	dmEmpty_Label2 = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label2->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label2, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble2 = new wxSpinCtrlDouble( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 0, -20.000000, 1 );
	dmColorOptionsCustom_SpinCtrlDouble2->SetMaxSize( wxSize( 100,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble2, 0, wxALL, 5 );
	
	dmColorOptionsCustom_OK_Label = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxT("Cautiously on normal water"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_OK_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_OK_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker3 = new wxColourPickerCtrl( dmColourOptionsCustom_Panel, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker3, 0, wxALL, 5 );
	
	dmEmpty_Label3 = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label3->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label3, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble3 = new wxSpinCtrlDouble( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 10, -22.500000, 1 );
	dmColorOptionsCustom_SpinCtrlDouble3->SetMaxSize( wxSize( 100,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble3, 0, wxALL, 5 );
	
	dmColorOptionsCustom_OKOnLowWater_Label = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxT("Cautiously on low water"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_OKOnLowWater_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_OKOnLowWater_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker4 = new wxColourPickerCtrl( dmColourOptionsCustom_Panel, wxID_ANY, wxColour( 128, 196, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker4, 0, wxALL, 5 );
	
	dmEmpty_Label4 = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_Label4->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmEmpty_Label4, 0, wxALL, 5 );
	
	dmColorOptionsCustom_SpinCtrlDouble4 = new wxSpinCtrlDouble( dmColourOptionsCustom_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 10, -25.000000, 1 );
	dmColorOptionsCustom_SpinCtrlDouble4->SetMaxSize( wxSize( 100,-1 ) );
	
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_SpinCtrlDouble4, 0, wxALL, 5 );
	
	dmColorOptionsCustom_Deep_Label = new wxStaticText( dmColourOptionsCustom_Panel, wxID_ANY, wxT("Deep, good to go"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColorOptionsCustom_Deep_Label->Wrap( -1 );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_Deep_Label, 0, wxALL, 5 );
	
	dmColorOptionsCustom_ColourPicker5 = new wxColourPickerCtrl( dmColourOptionsCustom_Panel, wxID_ANY, wxColour( 0, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsCustom_GridSizer->Add( dmColorOptionsCustom_ColourPicker5, 0, wxALL, 5 );
	
	
	dmColorOptionsCustom_Sizer->Add( dmColorOptionsCustom_GridSizer, 1, wxEXPAND, 5 );
	
	
	dmColourOptionsCustom_Panel->SetSizer( dmColorOptionsCustom_Sizer );
	dmColourOptionsCustom_Panel->Layout();
	dmColorOptionsCustom_Sizer->Fit( dmColourOptionsCustom_Panel );
	dmColourOptions_choisebook->AddPage( dmColourOptionsCustom_Panel, wxT("(default) Five depth ranges"), true );
	dmColourOptionsSliding_Panel = new wxPanel( dmColourOptions_choisebook, DM_viz_SLIDING_COLOUR, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmColourOptionsSliding_Panel->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );
	dmColourOptionsSliding_Panel->Enable( false );
	
	wxFlexGridSizer* dmColourOptionsSliding_GridSizer;
	dmColourOptionsSliding_GridSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmColourOptionsSliding_GridSizer->SetFlexibleDirection( wxBOTH );
	dmColourOptionsSliding_GridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmEmpty_staticText3 = new wxStaticText( dmColourOptionsSliding_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_staticText3->Wrap( -1 );
	dmColourOptionsSliding_GridSizer->Add( dmEmpty_staticText3, 0, wxALL, 5 );
	
	dmEmpty_staticText31 = new wxStaticText( dmColourOptionsSliding_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_staticText31->Wrap( -1 );
	dmColourOptionsSliding_GridSizer->Add( dmEmpty_staticText31, 0, wxALL, 5 );
	
	dmColourOptionsSliding_ShallowestLevelOfInterest_Label = new wxStaticText( dmColourOptionsSliding_Panel, wxID_ANY, wxT("Shallowest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsSliding_ShallowestLevelOfInterest_Label->Wrap( -1 );
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_ShallowestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_spinCtrlDouble = new wxSpinCtrlDouble( dmColourOptionsSliding_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 10, -20, 1 );
	dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_spinCtrlDouble->SetMaxSize( wxSize( 100,-1 ) );
	
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_spinCtrlDouble, 0, wxALL, 5 );
	
	dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_Label = new wxStaticText( dmColourOptionsSliding_Panel, wxID_ANY, wxT("Colour at shallowest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_Label->Wrap( -1 );
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColourOptionsSliding_ShallowestLevelOfInterest_ColourPicker = new wxColourPickerCtrl( dmColourOptionsSliding_Panel, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_ShallowestLevelOfInterest_ColourPicker, 0, wxALL, 5 );
	
	dmColourOptionsSliding_DeepestLevelOfInterest_Label = new wxStaticText( dmColourOptionsSliding_Panel, wxID_ANY, wxT("Deepest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsSliding_DeepestLevelOfInterest_Label->Wrap( -1 );
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_DeepestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColourOptionsSliding_DeepestLevelOfInterest_spinCtrlDouble = new wxSpinCtrlDouble( dmColourOptionsSliding_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 10, -30, 1 );
	dmColourOptionsSliding_DeepestLevelOfInterest_spinCtrlDouble->SetMaxSize( wxSize( 100,-1 ) );
	
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_DeepestLevelOfInterest_spinCtrlDouble, 0, wxALL, 5 );
	
	dmColourOptionsSliding_ColourAtDeepestLevelOfInterest_Label = new wxStaticText( dmColourOptionsSliding_Panel, wxID_ANY, wxT("Colour at deepest level of interest"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsSliding_ColourAtDeepestLevelOfInterest_Label->Wrap( -1 );
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_ColourAtDeepestLevelOfInterest_Label, 0, wxALL, 5 );
	
	dmColourOptionsSliding_ColourAtDeepestLevelOfInterest_ColourPicker = new wxColourPickerCtrl( dmColourOptionsSliding_Panel, wxID_ANY, wxColour( 0, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColourOptionsSliding_GridSizer->Add( dmColourOptionsSliding_ColourAtDeepestLevelOfInterest_ColourPicker, 0, wxALL, 5 );
	
	
	dmColourOptionsSliding_Panel->SetSizer( dmColourOptionsSliding_GridSizer );
	dmColourOptionsSliding_Panel->Layout();
	dmColourOptionsSliding_GridSizer->Fit( dmColourOptionsSliding_Panel );
	dmColourOptions_choisebook->AddPage( dmColourOptionsSliding_Panel, wxT("Sliding colour"), false );
	dmColourOptionsTwoColours_Panel = new wxPanel( dmColourOptions_choisebook, DM_viz_TWO_DEPTH_RANGES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* dmColorOptionsConstant_GridSizer;
	dmColorOptionsConstant_GridSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmColorOptionsConstant_GridSizer->SetFlexibleDirection( wxBOTH );
	dmColorOptionsConstant_GridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmEmpty_staticText4 = new wxStaticText( dmColourOptionsTwoColours_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_staticText4->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmEmpty_staticText4, 0, wxALL, 5 );
	
	dmEmpty_staticText5 = new wxStaticText( dmColourOptionsTwoColours_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty_staticText5->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmEmpty_staticText5, 0, wxALL, 5 );
	
	dmColourOptionsTwoColours_TooShallow_Label = new wxStaticText( dmColourOptionsTwoColours_Panel, wxID_ANY, wxT("Colour when too shallow "), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsTwoColours_TooShallow_Label->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmColourOptionsTwoColours_TooShallow_Label, 0, wxALL, 5 );
	
	dmColourOptionsTwoColours_TooShallow_ColourPicker = new wxColourPickerCtrl( dmColourOptionsTwoColours_Panel, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsConstant_GridSizer->Add( dmColourOptionsTwoColours_TooShallow_ColourPicker, 0, wxALL, 5 );
	
	dmColourOptionsTwoColours_DividingDepth_Label = new wxStaticText( dmColourOptionsTwoColours_Panel, wxID_ANY, wxT("Depth of deep enough"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsTwoColours_DividingDepth_Label->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmColourOptionsTwoColours_DividingDepth_Label, 0, wxALL, 5 );
	
	dmColourOptionsTwoColours_DividingDepth_spinCtrlDouble = new wxSpinCtrlDouble( dmColourOptionsTwoColours_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 10, -21.000000, 1 );
	dmColourOptionsTwoColours_DividingDepth_spinCtrlDouble->SetMaxSize( wxSize( 100,-1 ) );
	
	dmColorOptionsConstant_GridSizer->Add( dmColourOptionsTwoColours_DividingDepth_spinCtrlDouble, 0, wxALL, 5 );
	
	dmColourOptionsTwoColours_DeepEnough_Label = new wxStaticText( dmColourOptionsTwoColours_Panel, wxID_ANY, wxT("Colour when deep enough"), wxDefaultPosition, wxDefaultSize, 0 );
	dmColourOptionsTwoColours_DeepEnough_Label->Wrap( -1 );
	dmColorOptionsConstant_GridSizer->Add( dmColourOptionsTwoColours_DeepEnough_Label, 0, wxALL, 5 );
	
	dmColourOptionsTwoColours_DeepEnough_ColourPicker = new wxColourPickerCtrl( dmColourOptionsTwoColours_Panel, wxID_ANY, wxColour( 0, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	dmColorOptionsConstant_GridSizer->Add( dmColourOptionsTwoColours_DeepEnough_ColourPicker, 0, wxALL, 5 );
	
	
	dmColourOptionsTwoColours_Panel->SetSizer( dmColorOptionsConstant_GridSizer );
	dmColourOptionsTwoColours_Panel->Layout();
	dmColorOptionsConstant_GridSizer->Fit( dmColourOptionsTwoColours_Panel );
	dmColourOptions_choisebook->AddPage( dmColourOptionsTwoColours_Panel, wxT("Two colours (Go / No go)"), false );
	dmChartOptions_ColorRelief_Sizer->Add( dmColourOptions_choisebook, 1, wxEXPAND | wxALL, 5 );
	
	
	dmChartOptions_ColorRelief_panel->SetSizer( dmChartOptions_ColorRelief_Sizer );
	dmChartOptions_ColorRelief_panel->Layout();
	dmChartOptions_ColorRelief_Sizer->Fit( dmChartOptions_ColorRelief_panel );
	dmChartOptions_choicebook->AddPage( dmChartOptions_ColorRelief_panel, wxT("(default) Draw a Depth model  (\"Color relief\") chart"), true );
	dmChartOptions_Hillshade_panel = new wxPanel( dmChartOptions_choicebook, DM_viz_HILLSHADE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmChartOptions_Hillshade_panel->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );
	dmChartOptions_Hillshade_panel->Enable( false );
	
	wxFlexGridSizer* dmChartOptions_Hillshade_Sizer;
	dmChartOptions_Hillshade_Sizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmChartOptions_Hillshade_Sizer->SetFlexibleDirection( wxBOTH );
	dmChartOptions_Hillshade_Sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dm_Empty_staticText1 = new wxStaticText( dmChartOptions_Hillshade_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dm_Empty_staticText1->Wrap( -1 );
	dmChartOptions_Hillshade_Sizer->Add( dm_Empty_staticText1, 0, wxALL, 5 );
	
	dm_Empty_staticText2 = new wxStaticText( dmChartOptions_Hillshade_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dm_Empty_staticText2->Wrap( -1 );
	dmChartOptions_Hillshade_Sizer->Add( dm_Empty_staticText2, 0, wxALL, 5 );
	
	dmChartOptions_Hillshade_Azimuth_staticText = new wxStaticText( dmChartOptions_Hillshade_panel, wxID_ANY, wxT("Azimuth (default 315 degrees)"), wxDefaultPosition, wxDefaultSize, 0 );
	dmChartOptions_Hillshade_Azimuth_staticText->SetLabelMarkup( wxT("Azimuth (default 315 degrees)") );
	dmChartOptions_Hillshade_Azimuth_staticText->Wrap( -1 );
	dmChartOptions_Hillshade_Sizer->Add( dmChartOptions_Hillshade_Azimuth_staticText, 0, wxALL, 5 );
	
	dmChartOptions_Hillshade_Azimuth_spinCtrl = new wxSpinCtrl( dmChartOptions_Hillshade_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 315 );
	dmChartOptions_Hillshade_Sizer->Add( dmChartOptions_Hillshade_Azimuth_spinCtrl, 0, wxALL, 5 );
	
	dmChartOptions_Hillshade_Altitude_staticText = new wxStaticText( dmChartOptions_Hillshade_panel, wxID_ANY, wxT("Altitude (default 45 degrees)"), wxDefaultPosition, wxDefaultSize, 0 );
	dmChartOptions_Hillshade_Altitude_staticText->SetLabelMarkup( wxT("Altitude (default 45 degrees)") );
	dmChartOptions_Hillshade_Altitude_staticText->Wrap( -1 );
	dmChartOptions_Hillshade_Sizer->Add( dmChartOptions_Hillshade_Altitude_staticText, 0, wxALL, 5 );
	
	dmChartOptions_Hillshade_Altitude_spinCtrl = new wxSpinCtrl( dmChartOptions_Hillshade_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 90, 45 );
	dmChartOptions_Hillshade_Sizer->Add( dmChartOptions_Hillshade_Altitude_spinCtrl, 0, wxALL, 5 );
	
	
	dmChartOptions_Hillshade_panel->SetSizer( dmChartOptions_Hillshade_Sizer );
	dmChartOptions_Hillshade_panel->Layout();
	dmChartOptions_Hillshade_Sizer->Fit( dmChartOptions_Hillshade_panel );
	dmChartOptions_choicebook->AddPage( dmChartOptions_Hillshade_panel, wxT("Draw a Hillshade chart"), false );
	dmChartOptions_PlainImage_panel = new wxPanel( dmChartOptions_choicebook, DM_viz_NONE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmChartOptions_PlainImage_Sizer;
	dmChartOptions_PlainImage_Sizer = new wxBoxSizer( wxVERTICAL );
	
	dm_Empty_staticText = new wxStaticText( dmChartOptions_PlainImage_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dm_Empty_staticText->Wrap( -1 );
	dmChartOptions_PlainImage_Sizer->Add( dm_Empty_staticText, 0, wxALL, 5 );
	
	dmChartOptions_PlainImage_staticText = new wxStaticText( dmChartOptions_PlainImage_panel, wxID_ANY, wxT("The image is drawn coordinates transformed to World Mercator, \nbut no other handling is made."), wxDefaultPosition, wxDefaultSize, 0 );
	dmChartOptions_PlainImage_staticText->Wrap( -1 );
	dmChartOptions_PlainImage_Sizer->Add( dmChartOptions_PlainImage_staticText, 0, wxALL, 5 );
	
	
	dmChartOptions_PlainImage_panel->SetSizer( dmChartOptions_PlainImage_Sizer );
	dmChartOptions_PlainImage_panel->Layout();
	dmChartOptions_PlainImage_Sizer->Fit( dmChartOptions_PlainImage_panel );
	dmChartOptions_choicebook->AddPage( dmChartOptions_PlainImage_panel, wxT("Draw a Plain image data chart"), false );
	dmChartOptions_Sizer->Add( dmChartOptions_choicebook, 1, wxEXPAND | wxALL, 5 );
	
	
	dmChartOptions_Panel->SetSizer( dmChartOptions_Sizer );
	dmChartOptions_Panel->Layout();
	dmChartOptions_Sizer->Fit( dmChartOptions_Panel );
	dmTabChooser->AddPage( dmChartOptions_Panel, wxT("Chart drawing options"), true );
	dmWaterLevel_Panel = new wxPanel( dmTabChooser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmWaterLevel_Panel->Hide();
	
	wxFlexGridSizer* dmWaterLevel_Sizer;
	dmWaterLevel_Sizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	dmWaterLevel_Sizer->SetFlexibleDirection( wxBOTH );
	dmWaterLevel_Sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	dmEmpty = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty->Wrap( -1 );
	dmWaterLevel_Sizer->Add( dmEmpty, 0, wxALL, 5 );
	
	dmEmpty1 = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty1->Wrap( -1 );
	dmWaterLevel_Sizer->Add( dmEmpty1, 0, wxALL, 5 );
	
	dmWaterLevel_CurrentWaterLevel_Label = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxT("Current water level"), wxDefaultPosition, wxDefaultSize, 0 );
	dmWaterLevel_CurrentWaterLevel_Label->Wrap( -1 );
	dmWaterLevel_Sizer->Add( dmWaterLevel_CurrentWaterLevel_Label, 0, wxALL, 5 );
	
	dmWaterLevel_CurrentWaterLevel_spinCtrlDouble = new wxSpinCtrlDouble( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10, 10, 0.000000, 0.5 );
	dmWaterLevel_CurrentWaterLevel_spinCtrlDouble->SetMaxSize( wxSize( 100,-1 ) );
	
	dmWaterLevel_Sizer->Add( dmWaterLevel_CurrentWaterLevel_spinCtrlDouble, 0, wxALL, 5 );
	
	dmEmpty2 = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty2->Wrap( -1 );
	dmWaterLevel_Sizer->Add( dmEmpty2, 0, wxALL, 5 );
	
	dmEmpty3 = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmEmpty3->Wrap( -1 );
	dmWaterLevel_Sizer->Add( dmEmpty3, 0, wxALL, 5 );
	
	dmWaterLevel_VerticalReferenceSystemOffset_Label = new wxStaticText( dmWaterLevel_Panel, wxID_ANY, wxT("Vertical reference system offset"), wxDefaultPosition, wxDefaultSize, 0 );
	dmWaterLevel_VerticalReferenceSystemOffset_Label->Wrap( -1 );
	dmWaterLevel_Sizer->Add( dmWaterLevel_VerticalReferenceSystemOffset_Label, 0, wxALL, 5 );
	
	dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble = new wxSpinCtrlDouble( dmWaterLevel_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10, 10, 0, 0.5 );
	dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble->SetMaxSize( wxSize( 100,-1 ) );
	
	dmWaterLevel_Sizer->Add( dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble, 0, wxALL, 5 );
	
	
	dmWaterLevel_Panel->SetSizer( dmWaterLevel_Sizer );
	dmWaterLevel_Panel->Layout();
	dmWaterLevel_Sizer->Fit( dmWaterLevel_Panel );
	dmTabChooser->AddPage( dmWaterLevel_Panel, wxT("Water level"), false );
	dmAbout_Panel = new wxPanel( dmTabChooser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dmAbout_Sizer;
	dmAbout_Sizer = new wxBoxSizer( wxHORIZONTAL );
	
	dmAbout_Icon_Panel = new wxPanel( dmAbout_Panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	dmAbout_Icon_Panel->SetMaxSize( wxSize( 40,-1 ) );
	
	wxBoxSizer* dmAbout_Icon_Sizer;
	dmAbout_Icon_Sizer = new wxBoxSizer( wxVERTICAL );
	
	dmAbout_icon_bitmap = new wxStaticBitmap( dmAbout_Icon_Panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	dmAbout_Icon_Sizer->Add( dmAbout_icon_bitmap, 0, wxALL, 5 );
	
	
	dmAbout_Icon_Panel->SetSizer( dmAbout_Icon_Sizer );
	dmAbout_Icon_Panel->Layout();
	dmAbout_Icon_Sizer->Fit( dmAbout_Icon_Panel );
	dmAbout_Sizer->Add( dmAbout_Icon_Panel, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* dmAbout_Sizer2;
	dmAbout_Sizer2 = new wxBoxSizer( wxVERTICAL );
	
	dmEmpty_label = new wxStaticText( dmAbout_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	dmEmpty_label->Wrap( -1 );
	dmAbout_Sizer2->Add( dmEmpty_label, 0, wxALL, 5 );
	
	dmAbout_name_staticText = new wxStaticText( dmAbout_Panel, wxID_ANY, wxT("Depth model by LIVI"), wxDefaultPosition, wxDefaultSize, 0 );
	dmAbout_name_staticText->Wrap( 300 );
	dmAbout_Sizer2->Add( dmAbout_name_staticText, 0, wxALL, 5 );
	
	dmAbout_version_staticText = new wxStaticText( dmAbout_Panel, wxID_ANY, wxT("v. (undefined now, set in code)"), wxDefaultPosition, wxDefaultSize, 0 );
	dmAbout_version_staticText->Wrap( -1 );
	dmAbout_Sizer2->Add( dmAbout_version_staticText, 0, wxALL, 5 );
	
	dmAbout_description_staticText = new wxStaticText( dmAbout_Panel, wxID_ANY, wxT("Interpret'n draw depth model charts"), wxDefaultPosition, wxDefaultSize, 0 );
	dmAbout_description_staticText->Wrap( 300 );
	dmAbout_Sizer2->Add( dmAbout_description_staticText, 0, wxALL, 5 );
	
	dmAbout_copyright_staticText = new wxStaticText( dmAbout_Panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	dmAbout_copyright_staticText->Wrap( 300 );
	dmAbout_Sizer2->Add( dmAbout_copyright_staticText, 0, wxALL, 5 );
	
	dmAbout_development_team_staticText = new wxStaticText( dmAbout_Panel, wxID_ANY, wxT("depth model dev team"), wxDefaultPosition, wxDefaultSize, 0 );
	dmAbout_development_team_staticText->Wrap( -1 );
	dmAbout_Sizer2->Add( dmAbout_development_team_staticText, 0, wxALL, 5 );
	
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
	dmPictureImport_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LIVIDMUI_DLG::OnImageFileChange ), NULL, this );
	dmPictureImport_GenerateImage_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnGenerateImage ), NULL, this );
	dmPictureImport_ClearImage_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnClearImage ), NULL, this );
	dmChartOptions_choicebook->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( LIVIDMUI_DLG::OnChartTypeChange ), NULL, this );
	dmColourOptions_choisebook->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( LIVIDMUI_DLG::OnColourSchemaChange ), NULL, this );
	dmColourOptionsUserFile_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LIVIDMUI_DLG::OnUserColourFileChange ), NULL, this );
	dmWaterLevel_CurrentWaterLevel_spinCtrlDouble->Connect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( LIVIDMUI_DLG::OnCurrentWaterLevelChange ), NULL, this );
	dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble->Connect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( LIVIDMUI_DLG::OnVerticalReferenceSystemOffsetChange ), NULL, this );
	dm_AboutWxWidgets_Button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnAboutWxWidgets ), NULL, this );
}

LIVIDMUI_DLG::~LIVIDMUI_DLG()
{
	// Disconnect Events
	dmPictureImport_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LIVIDMUI_DLG::OnImageFileChange ), NULL, this );
	dmPictureImport_GenerateImage_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnGenerateImage ), NULL, this );
	dmPictureImport_ClearImage_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnClearImage ), NULL, this );
	dmChartOptions_choicebook->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( LIVIDMUI_DLG::OnChartTypeChange ), NULL, this );
	dmColourOptions_choisebook->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( LIVIDMUI_DLG::OnColourSchemaChange ), NULL, this );
	dmColourOptionsUserFile_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LIVIDMUI_DLG::OnUserColourFileChange ), NULL, this );
	dmWaterLevel_CurrentWaterLevel_spinCtrlDouble->Disconnect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( LIVIDMUI_DLG::OnCurrentWaterLevelChange ), NULL, this );
	dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble->Disconnect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( LIVIDMUI_DLG::OnVerticalReferenceSystemOffsetChange ), NULL, this );
	dm_AboutWxWidgets_Button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LIVIDMUI_DLG::OnAboutWxWidgets ), NULL, this );
	
}
