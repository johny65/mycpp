#ifndef _PDFDOCUMENT_H_
#define _PDFDOCUMENT_H_

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <podofo/podofo.h>

using namespace std;
using namespace PoDoFo;

class PDFDocument {
private:
	PdfMemDocument pdf;
	bool extract_bookmarks(ofstream &salida, PdfOutlineItem* pItem, int level);
	string escape(const string &utf8);
	void clear_bookmarks();
	string* split(const string &s);
	
public:
	PDFDocument(const char *archivo);
	~PDFDocument();
	bool ExtraerBookmarks(ofstream &salida);
	bool InsertarBookmarks(ifstream &entrada);
	void EliminarBookmarks();

};

#endif
