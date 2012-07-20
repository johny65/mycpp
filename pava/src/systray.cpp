#include "systray.h"
#include <wx/msgdlg.h>
#include <wx/settings.h>

miTimer reloj;

systray::systray()
{
	calentando = false;
	
	//menú inicial
	mnuSimple = new wxMenu();
	mnuSimple->Append(MNUSALIR, wxT("&Salir"));
	
	//menú calentando
	mnuMas = new wxMenu();
	//elemento en negrita
	wxFont *fuente = new wxFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	fuente->SetWeight(wxFONTWEIGHT_BOLD);
	wxMenuItem *it = new wxMenuItem(mnuMas, MNUPROGRESO, wxT("&Ver progreso"));
	it->SetFont(*fuente);
	mnuMas->Append(it);
	delete fuente;
	mnuMas->Append(MNUCANCELAR, wxT("&Cancelar control"));
	mnuMas->AppendSeparator();
	mnuMas->Append(MNUSALIR, wxT("&Salir"));
	
	//conectar eventos
	Connect(wxEVT_TASKBAR_RIGHT_UP, wxTaskBarIconEventHandler(systray::ClickDerecho));
	Connect(MNUSALIR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(systray::mnuIcoSalirClick));
}

void systray::PonerIcono()
{
	wxIcon *ic = new wxIcon(wxT("img/ico16x16.png"), wxBITMAP_TYPE_PNG);
	SetIcon(*ic, wxT("Pava - Inactivo"));
	delete ic;
}

void systray::SacarIcono()
{
	RemoveIcon();
}

void systray::ActivarPava()
{
	calentando = true;
}

void systray::DesactivarPava()
{
	calentando = false;
}

void systray::ClickDerecho(wxTaskBarIconEvent &event) //click derecho en el ícono
{
	if (calentando)
		PopupMenu(mnuMas);
	else
		PopupMenu(mnuSimple);
}

void systray::mnuIcoSalirClick(wxCommandEvent &evento) //menú Salir
{
	RemoveIcon();
	exit(0);
}


//-----------------------------------------------------------------

void miTimer::Iniciar(int tiempo, bool oneshot, wxWindow *w)
{
	main = w;
	Start(tiempo, oneshot);
}

void miTimer::Notify()
{
	Stop();
	wxMessageBox("Está la pava");
	//main->iconobandeja->DesactivarPava();
	main->Show();
}
