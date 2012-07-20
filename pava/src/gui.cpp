///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

frmMainClass::frmMainClass( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,150 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	cmdPoner = new wxButton( m_panel1, wxID_ANY, "¡Poné la pava!", wxDefaultPosition, wxDefaultSize, 0 );
	cmdPoner->SetMinSize( wxSize( 150,50 ) );
	
	bSizer3->Add( cmdPoner, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer2->Add( bSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	m_menubar1 = new wxMenuBar( 0 );
	mnuArchivo = new wxMenu();
	wxMenuItem* mnuOpciones;
	mnuOpciones = new wxMenuItem( mnuArchivo, wxID_ANY, wxString( wxT("&Opciones") ) , wxEmptyString, wxITEM_NORMAL );
	mnuArchivo->Append( mnuOpciones );
	
	wxMenuItem* m_separator1;
	m_separator1 = mnuArchivo->AppendSeparator();
	
	wxMenuItem* mnuSalir;
	mnuSalir = new wxMenuItem( mnuArchivo, wxID_ANY, wxString( wxT("&Salir") ) , wxEmptyString, wxITEM_NORMAL );
	mnuArchivo->Append( mnuSalir );
	
	m_menubar1->Append( mnuArchivo, wxT("&Archivo") ); 
	
	mnuAyuda = new wxMenu();
	wxMenuItem* mnuF1;
	mnuF1 = new wxMenuItem( mnuAyuda, wxID_ANY, "A&yuda rápida\t'F1'", wxEmptyString, wxITEM_NORMAL );
	mnuAyuda->Append( mnuF1 );
	
	wxMenuItem* m_separator2;
	m_separator2 = mnuAyuda->AppendSeparator();
	
	wxMenuItem* mnuAcerca;
	mnuAcerca = new wxMenuItem( mnuAyuda, wxID_ANY, wxString( wxT("&Acerca de") ) , wxEmptyString, wxITEM_NORMAL );
	mnuAyuda->Append( mnuAcerca );
	
	m_menubar1->Append( mnuAyuda, wxT("A&yuda") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( frmMainClass::AlCerrar ) );
	cmdPoner->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMainClass::cmdPonerClick ), NULL, this );
	this->Connect( mnuOpciones->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuOpcionesClick ) );
	this->Connect( mnuSalir->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuSalirClick ) );
	this->Connect( mnuF1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuF1Click ) );
	this->Connect( mnuAcerca->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuAcercaClick ) );
}

frmMainClass::~frmMainClass()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( frmMainClass::AlCerrar ) );
	cmdPoner->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMainClass::cmdPonerClick ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuOpcionesClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuSalirClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuF1Click ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMainClass::mnuAcercaClick ) );
	
}

frmAcercaClass::frmAcercaClass( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap1 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("img/pava.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_bitmap1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Pava"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_staticText2->SetFont( wxFont( 20, 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6->Add( m_staticText2, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, "Versión 1.0\nPara poner la pava tranquilo y hacer otras\ncosas sin tener miedo a que se pase el agua.\nCreado por Juan Bertinetti", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText1->Wrap( -1 );
	bSizer6->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cmdCerrar = new wxButton( this, wxID_ANY, wxT("Cerrar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( cmdCerrar, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );
	
	// Connect Events
	cmdCerrar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmAcercaClass::cmdCerrarClick ), NULL, this );
}

frmAcercaClass::~frmAcercaClass()
{
	// Disconnect Events
	cmdCerrar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmAcercaClass::cmdCerrarClick ), NULL, this );
	
}
