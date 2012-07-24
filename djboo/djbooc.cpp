#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

/*
djvused -e 'set-outline outline' e-text.djvu -s
*/

using namespace std;

///Elimina espacios al inicio y fin de una cadena.
/**
 * Elimina todos los caracteres de espacio y caracteres \c CR (<i>Carriage
 * Return</i>) que se encuentren al inicio y al final de una cadena.
 *
 * \param Cadena Cadena a procesar.
 * \return Una nueva cadena sin espacios en los extremos.
 */
string trim(const string &Cadena)
{
	unsigned int i, j;
	for (i=0; i<Cadena.length(); ++i){
		if (Cadena[i] != ' ' && Cadena[i] != char(13)) break;
	}
	for (j=Cadena.length()-1; j>=0; --j){
		if (Cadena[j] != ' ' && Cadena[j] != char(13)) break;
	}
	string res = Cadena.substr(i, j+1);
	return res;
}


vector<string> cargar_archivo(const char *filename)
{
	vector<string> archivo;
	
	ifstream entrada(filename);
	if (!entrada.good()){
		cout<<"Error al abrir el archivo.\n";
		return archivo;
	}
	
	string l;
	while (getline(entrada, l))
		archivo.push_back(l);
	entrada.close();

	return archivo;
}

bool guardar_outlines()
{
	int ret = system("djvused -e 'set-outline outline' e-text.djvu -s");
	return !ret;
}

void procesar_outline(const vector<string> &entrada, const char *archivo_salida)
{
	ofstream salida(archivo_salida, ios::trunc);
	salida<<"(bookmarks";
	string l;
	int nivel = -1;
	for (size_t i=0; i<entrada.size(); ++i){
		l = entrada[i];
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
}

int main(int argc, char *argv[]){

	/* nombres de archivo por defecto: */
	char DEFAULT_IN[] = "outline.txt";
	char DEFAULT_OUT[] = "out.txt";
	
	int o;
	char *filename_in = DEFAULT_IN;
	char *filename_out = DEFAULT_OUT;
	
	#ifdef DEBUG
		opterr = 1;
	#else
		opterr = 0; //desactivo mensajes de error predeterminados
	#endif

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
	
}
