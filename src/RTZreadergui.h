///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class m_Dialog
///////////////////////////////////////////////////////////////////////////////
class m_Dialog : public wxDialog
{
	private:

	protected:
		wxButton* m_button3111;
		wxStaticText* m_staticText15111;
		wxButton* m_button31112;
		wxButton* m_button31113;
		wxStaticLine* m_staticline7;
		wxStaticLine* m_staticline71;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText7;
		wxStaticLine* m_staticline5;
		wxStaticLine* m_staticline8;
		wxStaticText* m_staticText12;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnEditGPX( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnValidate( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxButton* m_button311131;
		wxTextCtrl* m_Route;
		wxChoice* m_choiceSchema;
		wxTextCtrl* m_textCtrlShowResult;

		m_Dialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("RTZreader"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

		~m_Dialog();

};

