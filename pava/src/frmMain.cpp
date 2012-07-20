#include "frmMain.h"
#include "frmAcerca.h"
#include <cstdlib>

frmMain::frmMain( wxWindow* parent )
:
frmMainClass( parent )
{
	SetIcon(wxIcon(wxT("img/ico16x16.png"), wxBITMAP_TYPE_PNG));

	//ícono en la bandeja del sistema
	iconobandeja = new systray();
	iconobandeja->PonerIcono();
	
}

void frmMain::cmdPonerClick( wxCommandEvent& event )
{
	//controla 5 minutos
	reloj.Iniciar(5000, false, this);
	iconobandeja->ActivarPava();
	Hide();
}

void frmMain::mnuOpcionesClick( wxCommandEvent& event )
{
	// TODO: Implement mnuOpcionesClick
}

void frmMain::mnuSalirClick( wxCommandEvent& event )
{
	Close();
}

void frmMain::mnuF1Click( wxCommandEvent& event )
{
	// TODO: Implement mnuF1Click
}

void frmMain::mnuAcercaClick( wxCommandEvent& event )
{
	frmAcerca *acerca = new frmAcerca(this);
	acerca->ShowModal();
	acerca->Destroy();
}

void frmMain::AlCerrar( wxCloseEvent& event )
{
	iconobandeja->SacarIcono();
	exit(0);
}
