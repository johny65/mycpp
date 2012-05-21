
//jjConfig versión 0.4
//
//Clase simple para guardar opciones de configuración en un archivo.
//
//Creado por Juan Bertinetti.

#ifndef _JJCONFIG_H_
#define _JJCONFIG_H_

#include <map>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

/// Clase jjConfig
/**
 * Clase principal. Para usar jjConfig hay que crear un objeto de tipo jjConfig
 * y luego trabajar con sus métodos públicos. Al instanciar el objeto se cargan
 * las opciones de configuración leídas de un archivo a memoria.
 */
class jjConfig {
private:
	map<string, string> data; ///< Mapa que guarda los datos
	string file; ///< Indica la ruta del archivo de configuración en disco
	
	bool cargar_datos();
	
	/* funciones de utilería: */
	string& trim(string&);
	int str2int(string&);
	unsigned int str2uint(string&);
	string int2str(int);
	string uint2str(unsigned int);
	double str2dbl(string&);
	string dbl2str(double);
	
public:
	jjConfig(string);
	bool Guardar();

	/* setters: */
	void SetValor(string, string);
	void SetValor(string, int);
	void SetValor(string, unsigned int);
	void SetValor(string, double);
	
	/* getters: */
	string Valor(string, string);
	int ValorInt(string, int);
	unsigned int ValorUInt(string, unsigned int);
	double ValorDouble(string, double);
	
};

#endif
