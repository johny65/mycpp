///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class frmMainClass
///////////////////////////////////////////////////////////////////////////////
class frmMainClass : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxButton* cmdPoner;
		wxMenuBar* m_menubar1;
		wxMenu* mnuArchivo;
		wxMenu* mnuAyuda;
		
		// Virtual event handlers, overide them in your derived class
		virtual void AlCerrar( wxCloseEvent& event ) { event.Skip(); }
		virtual void cmdPonerClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void mnuOpcionesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void mnuSalirClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void mnuF1Click( wxCommandEvent& event ) { event.Skip(); }
		virtual void mnuAcercaClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		frmMainClass( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Para poner la pava tranquilo..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		~frmMainClass();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class frmAcercaClass
///////////////////////////////////////////////////////////////////////////////
class frmAcercaClass : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmap1;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText1;
		wxButton* cmdCerrar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void cmdCerrarClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		frmAcercaClass( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Acerca de Pava"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~frmAcercaClass();
	
};

#endif //__gui__
