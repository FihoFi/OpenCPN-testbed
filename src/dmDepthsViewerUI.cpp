///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dmDepthsViewerUI.h"

///////////////////////////////////////////////////////////////////////////

dmDepthsViewerFrame::dmDepthsViewerFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	dm_coordinates_staticText = new wxStaticText( this, wxID_ANY, wxT("Depth (model+waterLevel)"), wxDefaultPosition, wxDefaultSize, 0 );
	dm_coordinates_staticText->Wrap( -1 );
	bSizer1->Add( dm_coordinates_staticText, 0, wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

dmDepthsViewerFrame::~dmDepthsViewerFrame()
{
}

dmDepthsViewerPanel::dmDepthsViewerPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	dm_coordinates_staticText = new wxStaticText( this, wxID_ANY, wxT("Depth (model+waterLevel)"), wxDefaultPosition, wxDefaultSize, 0 );
	dm_coordinates_staticText->Wrap( -1 );
	bSizer1->Add( dm_coordinates_staticText, 0, wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

dmDepthsViewerPanel::~dmDepthsViewerPanel()
{
}

dmDepthsViewerDialog::dmDepthsViewerDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	dm_coordinates_staticText = new wxStaticText( this, wxID_ANY, wxT("Depth (model+waterLevel)"), wxDefaultPosition, wxDefaultSize, 0 );
	dm_coordinates_staticText->Wrap( -1 );
	bSizer1->Add( dm_coordinates_staticText, 0, wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	this->Centre( wxBOTH );
}

dmDepthsViewerDialog::~dmDepthsViewerDialog()
{
}

dmDepthsViewerToolBar::dmDepthsViewerToolBar( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxToolBar( parent, id, pos, size, style )
{
	SetToolSeparation( 10 );

	dm_coordinates_staticText = new wxStaticText( this, wxID_ANY, wxT("Depth (model+waterLevel)"), wxDefaultPosition, wxDefaultSize, 0 );
	dm_coordinates_staticText->Wrap( -1 );
	AddControl( dm_coordinates_staticText );

	Realize();
}

dmDepthsViewerToolBar::~dmDepthsViewerToolBar()
{
}
