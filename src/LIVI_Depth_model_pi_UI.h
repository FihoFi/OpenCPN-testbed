///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul 11 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LIVI_DEPTH_MODEL_PI_UI_H__
#define __LIVI_DEPTH_MODEL_PI_UI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/filepicker.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/wrapsizer.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/choicebk.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define DM_viz_COLOR_RELIEF 1000
#define DM_viz_USER_FILE 1001
#define DM_viz_FIVE_DEPTH_RANGES 1002
#define DM_viz_SLIDING_COLOUR 1003
#define DM_viz_TWO_DEPTH_RANGES 1004
#define DM_viz_HILLSHADE 1005
#define DM_viz_NONE 1006

///////////////////////////////////////////////////////////////////////////////
/// Class LIVIDMUI_DLG
///////////////////////////////////////////////////////////////////////////////
class LIVIDMUI_DLG : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* dmTabChooser;
		wxPanel* dmFileImport_Panel;
		wxStaticText* dmDepthModelFile_staticText;
		wxFilePickerCtrl* dmPictureImport_filePicker;
		wxStaticText* dmPictureImport_CurrentlyDrawn_staticText;
		wxStaticText* dmPictureImport_Status_staticText;
		wxStaticText* dmPictureImport_ToGenerate_staticText;
		wxButton* dmPictureImport_GenerateImage_button;
		wxButton* dmPictureImport_ClearImage_button;
		wxPanel* dmChartOptions_Panel;
		wxStaticText* dmChartOptions_staticText;
		wxChoicebook* dmChartOptions_choicebook;
		wxPanel* dmChartOptions_ColorRelief_panel;
		wxStaticText* dmChartOptions_ColourRelief_staticText;
		wxStaticText* dmColourOptions_SelectShema_Label;
		wxChoicebook* dmColourOptions_choisebook;
		wxPanel* dmColourOptionsUserFile_Panel;
		wxStaticText* dmEmpty_staticText1;
		wxStaticText* dmColourOptionsUserFile_giveFile_staticText;
		wxFilePickerCtrl* dmColourOptionsUserFile_filePicker;
		wxStaticText* dmColourOptionsUserFile_help_staticText;
		wxPanel* dmColourOptionsCustom_Panel;
		wxStaticText* dmEmpty_staticText2;
		wxStaticText* dmColorOptionsCustom_Label;
		wxStaticText* dmColorOptionsCustom_TooShallow_Label;
		wxColourPickerCtrl* dmColorOptionsCustom_ColourPicker1;
		wxStaticText* dmEmpty_Label1;
		wxSpinCtrlDouble* dmColorOptionsCustom_SpinCtrlDouble1;
		wxStaticText* dmColorOptionsCustom_ABitRisky_Label;
		wxColourPickerCtrl* dmColorOptionsCustom_ColourPicker2;
		wxStaticText* dmEmpty_Label2;
		wxSpinCtrlDouble* dmColorOptionsCustom_SpinCtrlDouble2;
		wxStaticText* dmColorOptionsCustom_OK_Label;
		wxColourPickerCtrl* dmColorOptionsCustom_ColourPicker3;
		wxStaticText* dmEmpty_Label3;
		wxSpinCtrlDouble* dmColorOptionsCustom_SpinCtrlDouble3;
		wxStaticText* dmColorOptionsCustom_OKOnLowWater_Label;
		wxColourPickerCtrl* dmColorOptionsCustom_ColourPicker4;
		wxStaticText* dmEmpty_Label4;
		wxSpinCtrlDouble* dmColorOptionsCustom_SpinCtrlDouble4;
		wxStaticText* dmColorOptionsCustom_Deep_Label;
		wxColourPickerCtrl* dmColorOptionsCustom_ColourPicker5;
		wxPanel* dmColourOptionsSliding_Panel;
		wxStaticText* dmEmpty_staticText3;
		wxStaticText* dmEmpty_staticText31;
		wxStaticText* dmColourOptionsSliding_ShallowestLevelOfInterest_Label;
		wxSpinCtrlDouble* dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_spinCtrlDouble;
		wxStaticText* dmColourOptionsSliding_ColourAtShallowestLevelOfInterest_Label;
		wxColourPickerCtrl* dmColourOptionsSliding_ShallowestLevelOfInterest_ColourPicker;
		wxStaticText* dmColourOptionsSliding_DeepestLevelOfInterest_Label;
		wxSpinCtrlDouble* dmColourOptionsSliding_DeepestLevelOfInterest_spinCtrlDouble;
		wxStaticText* dmColourOptionsSliding_ColourAtDeepestLevelOfInterest_Label;
		wxColourPickerCtrl* dmColourOptionsSliding_ColourAtDeepestLevelOfInterest_ColourPicker;
		wxPanel* dmColourOptionsTwoColours_Panel;
		wxStaticText* dmEmpty_staticText4;
		wxStaticText* dmEmpty_staticText5;
		wxStaticText* dmColourOptionsTwoColours_TooShallow_Label;
		wxColourPickerCtrl* dmColourOptionsTwoColours_TooShallow_ColourPicker;
		wxStaticText* dmColourOptionsTwoColours_DividingDepth_Label;
		wxSpinCtrlDouble* dmColourOptionsTwoColours_DividingDepth_spinCtrlDouble;
		wxStaticText* dmColourOptionsTwoColours_DeepEnough_Label;
		wxColourPickerCtrl* dmColourOptionsTwoColours_DeepEnough_ColourPicker;
		wxPanel* dmChartOptions_Hillshade_panel;
		wxStaticText* dmChartOptions_Hillshade_paramInfoText;
		wxPanel* dmChartOptions_PlainImage_panel;
		wxStaticText* dm_Empty_staticText;
		wxStaticText* dmChartOptions_PlainImage_staticText;
		wxPanel* dmWaterLevel_Panel;
		wxStaticText* dmEmpty;
		wxStaticText* dmEmpty1;
		wxStaticText* dmWaterLevel_CurrentWaterLevel_Label;
		wxSpinCtrlDouble* dmWaterLevel_CurrentWaterLevel_spinCtrlDouble;
		wxStaticText* dmEmpty2;
		wxStaticText* dmEmpty3;
		wxStaticText* dmWaterLevel_VerticalReferenceSystemOffset_Label;
		wxSpinCtrlDouble* dmWaterLevel_VerticalReferenceSystemOffset_spinCtrlDouble;
		wxPanel* dmAbout_Panel;
		wxPanel* dmAbout_Icon_Panel;
		wxStaticBitmap* dmAbout_icon_bitmap;
		wxStaticText* dmEmpty_label;
		wxStaticText* dmAbout_name_staticText;
		wxStaticText* dmAbout_version_staticText;
		wxStaticText* dmAbout_description_staticText;
		wxStaticText* dmAbout_copyright_staticText;
		wxStaticText* dmAbout_development_team_staticText;
		wxButton* dm_AboutWxWidgets_Button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnImageFileChange( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnGenerateImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChartTypeChange( wxChoicebookEvent& event ) { event.Skip(); }
		virtual void OnColourSchemaChange( wxChoicebookEvent& event ) { event.Skip(); }
		virtual void OnUserColourFileChange( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnCurrentWaterLevelChange( wxSpinDoubleEvent& event ) { event.Skip(); }
		virtual void OnVerticalReferenceSystemOffsetChange( wxSpinDoubleEvent& event ) { event.Skip(); }
		virtual void OnAboutWxWidgets( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		LIVIDMUI_DLG( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~LIVIDMUI_DLG();
	
};

#endif //__LIVI_DEPTH_MODEL_PI_UI_H__
