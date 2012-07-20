#include <iostream>
#include <vector>
#include <string>
#include <fstream>

/*
djvused -e 'set-outline outline' e-text.djvu -s
*/

using namespace std;

string trim(string Cadena)
{
	unsigned int i, j;
	for (i=0; i<Cadena.length(); ++i){
		if (Cadena[i] != ' ' && Cadena[i] != char(13)) break;
	}
	for (j=Cadena.length()-1; j>=0; --j){
		if (Cadena[j] != ' ' && Cadena[j] != char(13)) break;
	}
	Cadena = Cadena.substr(i, j+1);
	return Cadena;
}

int main(int argc, char *argv[]){

	/* nombres de archivo por defecto: */
	char DEFAULT_IN[] = "outline.txt";
	char DEFAULT_OUT[] = "out.txt";
	
	int o;
	char *filename_in = DEFAULT_IN;
	char *filename_out = DEFAULT_OUT;
	opterr = 0; //desactivo mensajes de error predeterminados

	while ((o = getopt(argc, argv, "o:")) != -1){
		switch (o){
			case 'o': {
				filename_out = optarg;
				break;
			}
			case '?': {
				if (optopt == 'o')
					cout<<"Falta un parámetro: indicar nombre de archivo para la opción -o.\n";
				else
					cout<<"Opción inválida: -"<<char(optopt)<<endl;
				return EXIT_FAILURE;
			}
		}
	}
	if (optind == argc-1)
		filename_in = argv[optind];

	cout<<"Entrada: "<<filename_in<<endl;
	cout<<"Salida: "<<filename_out<<endl;
	return 0;
	
	
	
	ifstream entrada("outline.txt");
	if (!entrada.good()){
		cout<<"Error al abrir el archivo.\n";
		return EXIT_FAILURE;
	}
	ofstream salida("contents", ios::trunc);
	salida<<"(bookmarks";
	string l;
	vector<string> archivo;
	int nivel = -1;
	while (getline(entrada, l)){
		//contar tabs
		int canttabs = 0;
		int i = 0;
		while (l[i] == '\t'){
			canttabs++;
			i++;
		}
		size_t corte = l.find('|', i);
		if (canttabs == nivel) //sigue en el mismo nivel
			salida<<")\n(\""<<trim(l.substr(i, corte-i))<<"\" \"#"<<trim(l.substr(corte+1))<<"\"";
		else if (canttabs > nivel){ //nivel nuevo
			nivel = canttabs;
			salida<<"\n(\""<<trim(l.substr(i, corte-i))<<"\" \"#"<<trim(l.substr(corte+1))<<"\"";
		}
		else if (canttabs < nivel){ //nivel anterior
			int hasta = nivel-canttabs;
			nivel = canttabs;
			for (int j=0; j<hasta; ++j)
				salida<<")\n";
			salida<<")\n(\""<<trim(l.substr(i, corte-i))<<"\" \"#"<<trim(l.substr(corte+1))<<"\"";
		}
		
	}
	for (int i = 0; i<nivel+2; ++i)
		salida<<")\n";
	salida.close();
	return 0;
}
