#ifndef _PDFDOCUMENT_H_
#define _PDFDOCUMENT_H_

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <podofo/podofo.h>
#include "utils.h"

using namespace std;
using namespace PoDoFo;

class Outline {
public:
	int nivel;
	EPdfOutlineFormat format;
	string titulo;
	vector<int> dest;
};


class PDFDocument {
private:
	PdfMemDocument pdf;
	bool extract_bookmarks(ofstream &salida, PdfOutlineItem* pItem, int level);
	void clear_bookmarks();
	
public:
	vector<Outline> cargar_csv(const char *archivo);
	PDFDocument(const char *archivo);
	~PDFDocument();
	bool ExtraerBookmarks(ofstream &salida);
	bool InsertarBookmarks(vector<Outline> &books);
	void EliminarBookmarks();

};

#endif
