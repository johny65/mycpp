#include "pdfdocument.h"
#include "debug.h"


PDFDocument::PDFDocument(const char *archivo)
{
	pdf.Load(archivo);
	this->out_filename = "salida.pdf";
}


PDFDocument::~PDFDocument()
{
	
}

bool PDFDocument::extract_bookmarks(ofstream &salida, PdfOutlineItem* pItem, int level)
{
	PdfOutlines *pOutlines;
	int i;

	if (!pItem){
		pOutlines = this->pdf.GetOutlines(false);
		if (!pOutlines || !pOutlines->First()){
			return false;
		}
		pItem = pOutlines->First();
	}

	//level:
	salida<<level+1<<";";

	//options:
	EPdfOutlineFormat format = pItem->GetTextFormat();
	switch (format){
		case ePdfOutlineFormat_Italic:
			salida<<"I";
			break;
		case ePdfOutlineFormat_Bold:
			salida<<"B";
			break;
		case ePdfOutlineFormat_BoldItalic:
			salida<<"BI";
			break;
	}
	salida<<";";
	
	//title:
	PdfString title = pItem->GetTitle();
	//if (title.IsValid() && title.IsUnicode())
		//salida<<title.GetStringUtf8();
	//else if (title.IsValid() && !title.IsUnicode())
		//salida<<title.GetString();
	salida<<pdfboo_utils::escape(title.ToUnicode().GetStringUtf8());
	salida<<";";


	//destination:
	PdfDestination* pDest = pItem->GetDestination(&this->pdf);

	if (pDest){	// then it's a destination
	
		PdfPage* pPage = pDest->GetPage(&this->pdf);
		if (pPage)
			salida<<pPage->GetPageNumber();

		//type of destination:
		EPdfDestinationType type = pDest->GetType();
		switch (type){
			case ePdfDestinationType_XYZ: {
				salida<<" XYZ "<<pDest->GetLeft()<<" "<<pDest->GetTop()<<" "<<
					pDest->GetZoom();
				break;
			}
			case ePdfDestinationType_Fit: {
				salida<<" Fit";
				break;
			}
			case ePdfDestinationType_FitH: {
				salida<<" FitH "<<pDest->GetTop();
				break;
			}
			case ePdfDestinationType_FitV: {
				salida<<" FitV "<<pDest->GetLeft();
				break;
			}
			case ePdfDestinationType_FitR: {
				PdfRect r = pDest->GetRect();
				salida<<" FitR "<<r.GetLeft()<<" "<<r.GetBottom()<<" "<<
					r.GetLeft()+r.GetWidth()<<" "<<r.GetBottom()+r.GetHeight();
				break;
			}
			case ePdfDestinationType_FitB: {
				salida<<" FitB";
				break;
			}
			case ePdfDestinationType_FitBH: {
				salida<<" FitBH "<<pDest->GetTop();
				break;
			}
			case ePdfDestinationType_FitBV: {
				salida<<" FitBV "<<pDest->GetLeft();
				break;
			}
		}
	} //else it's one or more actions, not supported
	///<\todo ver los de las acciones
	salida<<endl;

	if (pItem->First())
		extract_bookmarks(salida, pItem->First(), level+1);
	
	if (pItem->Next())
		extract_bookmarks(salida, pItem->Next(), level);

	return true;
}

bool PDFDocument::ExtraerBookmarks(ofstream &salida)
{
	return extract_bookmarks(salida, NULL, 0);
}


bool PDFDocument::InsertarBookmarks(vector<Outline> &books)
{
	int current_level = books[0].nivel;
	if (current_level != 0)
		return false;
		
	clear_bookmarks();
	PdfOutlines *outs = this->pdf.GetOutlines(true); //empty

	//start with the root:
	
	PdfOutlineItem *raiz = outs->CreateRoot(books[0].titulo);
	PdfPage *pag = this->pdf.GetPage(books[0].dest[0] - 1);
	PdfDestination dest(pag);
	raiz->SetDestination(dest);
	raiz->SetTextFormat(books[0].format);

	/* guarda una referencia al último PdfOutlineItem de cada nivel, para poder
	 * seguir agregando outlines hijos (nivel siguiente) */
	vector<PdfOutlineItem*> arbol;
	arbol.push_back(raiz);

	dbg("Raíz creada.");
	
	PdfOutlineItem *current;
	for (size_t i=1; i<books.size(); ++i){

		current_level = books[i].nivel;
		
		PdfPage *pag = this->pdf.GetPage(books[i].dest[0] - 1);
		PdfDestination dest(pag);

		if (current_level == 0) //same level as the root
			current = arbol[0]->CreateNext(books[i].titulo, dest);
		else
			current = arbol[current_level-1]->CreateChild(books[i].titulo, dest);

		current->SetTitle(books[i].titulo);
		current->SetTextFormat(books[i].format);
		
		if (current_level == arbol.size())
			arbol.push_back(current);
		else
			arbol[current_level] = current;
		
		dbg("Siguiente...");
	}

	this->pdf.Write(this->out_filename);
	return true;
}

void PDFDocument::EliminarBookmarks()
{
	clear_bookmarks();
	this->pdf.Write(this->out_filename);
}

void PDFDocument::clear_bookmarks()
{
	//create new outlines (existing ones will be deleted)
	PdfOutlines *m_pOutlines = new PdfOutlines(&pdf.GetObjects());
	pdf.GetCatalog()->GetDictionary().AddKey("Outlines", m_pOutlines->GetObject()->Reference());
}

vector<Outline> PDFDocument::cargar_csv(const char *archivo)
{
	vector<Outline> res;
	ifstream entrada(archivo);
	if (!entrada.good())
		return res;
	
	string line;
	string *tokens;
	while (getline(entrada, line)){
		tokens = pdfboo_utils::split(line);
		Outline nuevo;
		nuevo.nivel = pdfboo_utils::str2int(tokens[0]) - 1;
		if (tokens[1] == "B")
			nuevo.format = ePdfOutlineFormat_Bold;
		else if (tokens[1] == "I")
			nuevo.format = ePdfOutlineFormat_Italic;
		else if (tokens[1] == "IB" || tokens[1] == "BI")
			nuevo.format = ePdfOutlineFormat_BoldItalic;
		else
			nuevo.format = ePdfOutlineFormat_Unknown;

		nuevo.titulo = tokens[2];
		
		stringstream ss(tokens[3]);
		int pag;
		ss>>pag;
		vector<int> v; v.push_back(pag);
		nuevo.dest = v;
		
		delete[] tokens;

		res.push_back(nuevo);

	}

	return res;
}
