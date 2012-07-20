#include <wx/msgdlg.h>
#include "Application.h"
#include "frmMain.h"

using namespace std;

bool mxApplication::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler); //inicializa formato png
		
	frmMain *main = new frmMain(NULL);
	main->Show();
	
}
