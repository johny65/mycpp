#include "pdfdocument.h"


PDFDocument::PDFDocument(const char *archivo)
{
	pdf.Load(archivo);
	
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
	salida<<escape(title.ToUnicode().GetStringUtf8());
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

bool PDFDocument::InsertarBookmarks(ifstream &entrada)
{
	clear_bookmarks();
	PdfOutlines *outs = this->pdf.GetOutlines(true); //empty

	int current_level = 0;
	int level;
	string *tokens;
	string ops, tit, dir, type;
	int pag;
	stringstream ss;

	
	string line;
	while (getline(entrada, line)){
		tokens = split(line);
		cout<<"Token0: "<<tokens[0]<<endl;
		ss.str(tokens[0]); ss>>level; level--;
		ops = tokens[1];
		tit = tokens[2];
		dir = tokens[3];
		delete[] tokens;

		ss.str(dir); ss>>pag>>type;

		//crear un destination con el constructor apropiado según el tipo
		//PdfDestination dest(
		//PdfOutlineItem( item(
		cout<<"Página: "<<pag<<endl;
		cout<<"Nivel: "<<level<<endl;
		cout<<"Título: "<<tit<<endl;
		cout<<"Tipo: "<<type<<endl;
	}

	return true;
}

string PDFDocument::escape(const string &utf8)
{
	string res;
	for (int i=0; i<utf8.size(); ++i){
		if (utf8.at(i) == '\\')
			res += "\\5C";
		else if (utf8.at(i) == ';')
			res += "\\3B";
		else
			res += utf8.at(i);
	}
	return res;
}

void PDFDocument::EliminarBookmarks()
{
	clear_bookmarks();
	this->pdf.Write("salida.pdf");
}

void PDFDocument::clear_bookmarks()
{
	//create new outlines (existing ones will be deleted)
	PdfOutlines *m_pOutlines = new PdfOutlines(&pdf.GetObjects());
	pdf.GetCatalog()->GetDictionary().AddKey("Outlines", m_pOutlines->GetObject()->Reference());
}

string* PDFDocument::split(const string &s)
{
	#define NUM 4
	string *res = new string[NUM];
	int pos = 0, ind = 0;

	for (int i=0; i<s.length(); ++i){
		if (s[i] == ';'){
			res[ind] = s.substr(pos, i-pos);
			pos = i + 1;
			ind++;
		}
	}
	res[ind] = s.substr(pos);

	return res;
}
