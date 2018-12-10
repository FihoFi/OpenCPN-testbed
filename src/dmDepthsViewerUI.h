///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul 11 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DMDEPTHSVIEWERUI_H__
#define __DMDEPTHSVIEWERUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/toolbar.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class dmDepthsViewerFrame
///////////////////////////////////////////////////////////////////////////////
class dmDepthsViewerFrame : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* dm_coordinates_staticText;
	
	public:
		
		dmDepthsViewerFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 179,321 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~dmDepthsViewerFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dmDepthsViewerPanel
///////////////////////////////////////////////////////////////////////////////
class dmDepthsViewerPanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* dm_coordinates_staticText;
	
	public:
		
		dmDepthsViewerPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 176,126 ), long style = wxTAB_TRAVERSAL ); 
		~dmDepthsViewerPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dmDepthsViewerDialog
///////////////////////////////////////////////////////////////////////////////
class dmDepthsViewerDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* dm_coordinates_staticText;
	
	public:
		
		dmDepthsViewerDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~dmDepthsViewerDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dmDepthsViewerToolBar
///////////////////////////////////////////////////////////////////////////////
class dmDepthsViewerToolBar : public wxToolBar 
{
	private:
	
	protected:
		wxStaticText* dm_coordinates_staticText;
	
	public:
		
		dmDepthsViewerToolBar( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 287,29 ), long style = 0 );
		~dmDepthsViewerToolBar();
	
};

#endif //__DMDEPTHSVIEWERUI_H__
