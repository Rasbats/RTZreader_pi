///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "RTZreadergui.h"

///////////////////////////////////////////////////////////////////////////

m_Dialog::m_Dialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer9111;
	bSizer9111 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer1311;
	bSizer1311 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer14111112;
	bSizer14111112 = new wxBoxSizer( wxVERTICAL );


	bSizer1311->Add( bSizer14111112, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );

	m_button3111 = new wxButton( this, wxID_ANY, wxT("Edit GPX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button3111->Hide();

	bSizer71->Add( m_button3111, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );

	m_staticText15111 = new wxStaticText( this, wxID_ANY, wxT("Files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15111->Wrap( -1 );
	m_staticText15111->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer71->Add( m_staticText15111, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_button31112 = new wxButton( this, wxID_ANY, wxT("Read RTZ"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_button31112, 0, wxALL|wxEXPAND, 5 );

	m_button31113 = new wxButton( this, wxID_ANY, wxT("Write RTZ"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_button31113, 0, wxALL|wxEXPAND, 5 );

	m_staticline7 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer71->Add( m_staticline7, 0, wxEXPAND | wxALL, 5 );

	m_staticline71 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer71->Add( m_staticline71, 0, wxEXPAND | wxALL, 5 );

	m_button311131 = new wxButton( this, wxID_ANY, wxT("Validate RTZ"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_button311131, 0, wxALL|wxEXPAND, 5 );


	bSizer1311->Add( bSizer71, 0, wxEXPAND, 5 );


	bSizer9->Add( bSizer1311, 0, wxEXPAND, 5 );


	bSizer9111->Add( bSizer9, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Options"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	m_staticText11->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer12->Add( m_staticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Route name :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer14->Add( m_staticText7, 0, wxALL, 5 );

	m_Route = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_Route, 0, wxALL, 5 );


	bSizer12->Add( bSizer14, 1, wxEXPAND, 5 );

	wxString m_choiceSchemaChoices[] = { wxT("RTZ_Schema_version_1_0"), wxT("RTZ_Schema_version_1_1"), wxT("RTZ_Schema_version_1_2"), wxEmptyString };
	int m_choiceSchemaNChoices = sizeof( m_choiceSchemaChoices ) / sizeof( wxString );
	m_choiceSchema = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSchemaNChoices, m_choiceSchemaChoices, 0 );
	m_choiceSchema->SetSelection( 0 );
	bSizer12->Add( m_choiceSchema, 0, wxALL, 5 );

	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );

	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Pass/Fail"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer91->Add( m_staticText12, 0, wxALL, 5 );

	m_textCtrlShowResult = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlShowResult->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	m_textCtrlShowResult->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer91->Add( m_textCtrlShowResult, 0, wxALL|wxEXPAND, 5 );


	bSizer12->Add( bSizer91, 1, wxEXPAND, 5 );


	bSizer10->Add( bSizer12, 1, wxEXPAND, 5 );


	bSizer9111->Add( bSizer10, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer9111 );
	this->Layout();
	bSizer9111->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( m_Dialog::OnClose ) );
	m_button3111->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnEditGPX ), NULL, this );
	m_button31112->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnImport ), NULL, this );
	m_button31113->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnExport ), NULL, this );
	m_button311131->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnValidate ), NULL, this );
}

m_Dialog::~m_Dialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( m_Dialog::OnClose ) );
	m_button3111->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnEditGPX ), NULL, this );
	m_button31112->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnImport ), NULL, this );
	m_button31113->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnExport ), NULL, this );
	m_button311131->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnValidate ), NULL, this );

}
