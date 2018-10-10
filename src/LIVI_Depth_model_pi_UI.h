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
#include <wx/stattext.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/choicebk.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


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
		wxPanel* dmColorOptions_Panel;
		wxStaticText* dmColorOptions_SelectShema_Label;
		wxChoicebook* dmColorOptions_Choisebook;
		wxPanel* dmColorOptionsCustom_Panel;
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
		wxPanel* dmColorOptionsSliding_Panel;
		wxStaticText* dmColorOptionsSliding_DeepestLevelOfInterest_Label;
		wxTextCtrl* dmColorOptionsSliding_DeepestLevelOfInterest_TextBox;
		wxStaticText* dmColorOptionsSliding_ColorAtDeepestLevelOfInterest_Label;
		wxColourPickerCtrl* dmColorOptionsSliding_ColorAtDeepestLevelOfInterest_ColorPicker;
		wxStaticText* dmColorOptionsSliding_ShallowestLevelOfInterest_Label;
		wxTextCtrl* dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_TextBox;
		wxStaticText* dmColorOptionsSliding_ColorAtShallowestLevelOfInterest_Label;
		wxColourPickerCtrl* dmColorOptionsSliding_ShallowestLevelOfInterest_ColourPicker;
		wxPanel* dmColorOptionsConstant_Panel;
		wxStaticText* dmColorOptionsConstant_SetStep_Label;
		wxTextCtrl* dmColorOptionsConstant_SetStep_TextCtrl;
		wxStaticText* dmColorOptionsConstant_ColorAtDeepestLevelOfInterest_Label1;
		wxColourPickerCtrl* dmColorOptionsConstant_ColorAtDeepestLevelOfInterest_ColourPicker;
		wxStaticText* dmColorOptionsConstant_ColorAtShallowestLevelOfInterest_Label1;
		wxColourPickerCtrl* dmColorOptionsConstant_ShallowestLevelOfInterest_ColourPicker;
		wxPanel* dmColorOptionsUserFile_Panel;
		wxStaticText* dmColorOptionsUserFile_giveFile_staticText;
		wxFilePickerCtrl* dmColorOptionsUserFile_filePicker;
		wxStaticText* dmColorOptionsUserFile_help_staticText;
		wxButton* dmColorOptions_Apply_Button;
		wxPanel* dmWaterLevel_Panel;
		wxStaticText* dmEmpty;
		wxStaticText* dmEmpty1;
		wxStaticText* dmWaterLevel_Label;
		wxTextCtrl* dmWaterLevel_textCtrl;
		wxPanel* dmAbout_Panel;
		wxPanel* dmAbout_Icon_Panel;
		wxStaticText* dmAbout_Label;
		wxButton* dmAbout_LIVIDMPlugin_Button;
		wxButton* dm_AboutWxWidgets_Button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFileImportFileChange( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnColorOptionsApplyButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutLIVIDepthModel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutWxWidgets( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		LIVIDMUI_DLG( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~LIVIDMUI_DLG();
	
};

#endif //__LIVI_DEPTH_MODEL_PI_UI_H__