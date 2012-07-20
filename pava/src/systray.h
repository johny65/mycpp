#ifndef SYSTRAYH
#define SYSTRAYH

#include <wx/taskbar.h>
#include <wx/timer.h>
#include <wx/menu.h>

class systray : public wxTaskBarIcon {
private:
	bool calentando;
	wxMenu *mnuSimple;
	wxMenu *mnuMas;
	
public:
	systray();
	void PonerIcono();
	void SacarIcono();
	void ActivarPava(); //cambia el ícono
	void DesactivarPava();
	//eventos
	void ClickDerecho(wxTaskBarIconEvent &evento); //click derecho en el ícono
	void mnuIcoSalirClick(wxCommandEvent &evento); //menú Salir
	
};

enum {
	MNUSALIR, MNUCANCELAR, MNUPROGRESO
};

//////////////////////////////////////////////////////////////////
class miTimer : public wxTimer {
private:
	wxWindow *main;
public:
	void Notify();
	void Iniciar(int tiempo, bool oneshot, wxWindow *w);
};

extern systray iconobandeja;
extern miTimer reloj;

#endif
