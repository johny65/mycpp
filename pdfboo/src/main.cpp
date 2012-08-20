#include <iostream>
#include "pdfdocument.h"

using namespace std;

int main(int argc, char **argv){

/*
	PdfMemDocument pdf("1.pdf");
	EPdfVersion ver = pdf.GetPdfVersion();
	//cout<<ver<<endl;

	PdfOutlines *outs = pdf.GetOutlines(false);
	if (outs == NULL)
		cout<<"El archivo no tiene marcadores.\n";
	else {
		PdfOutlineItem *i = outs->First();
		while (i != NULL){
			PdfString s = i->GetTitle();
			if (s.IsValid()){
				//cout<<"Nombre válido.\n";
				if (s.IsUnicode()){
					//cout<<"Está en Unicode\n";
					cout<<s.GetStringUtf8()<<endl;
				}
				else {
					//cout<<"No está en Unicode\n";
					cout<<s.GetString()<<endl;
				}
			}
			else
				cout<<"Nombre no válido.\n";

			if (i->First() == NULL)
				cout<<"No tiene hijos\n";
			i = i->Next();
		}
	}
*/
	//outs->First()->SetTitle("A ver!!!!!!");
	//pdf.Write("salida.pdf");

	PDFDocument pdf(argv[1]);
	//ofstream salida("out");
	//pdf.ExtraerBookmarks(salida);
	//salida.close();

	//pdf.EliminarBookmarks();

	//ifstream entrada("books");
	vector<Outline> v = pdf.cargar_csv("books");
	pdf.InsertarBookmarks(v);
	//entrada.close();
	
	return 0;
}
