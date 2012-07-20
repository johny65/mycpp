#include "frmAcerca.h"

frmAcerca::frmAcerca( wxWindow* parent )
:
frmAcercaClass( parent )
{

}

void frmAcerca::cmdCerrarClick( wxCommandEvent& event )
{
	EndModal(1);
}
