#include "jjConfig.h"

using namespace std;


/************************************************************************//**
 * \brief Constructor.
 *
 * Constructor de la clase. Recibe como parámetro la ruta en disco del archivo
 * de configuración que se usará. Carga todos los datos contenidos en el
 * archivo. Puede pasarse un archivo vacío o inexistente, en tales casos no
 * cargará ningún dato pero al momento de guardar creará los datos necesarios.
 * 
 * \param Archivo Ruta del archivo de configuración.
 * 
 ***************************************************************************/
jjConfig::jjConfig(string Archivo)
{
	this->file = Archivo;
	cargar_datos();
}


/************************************************************************//**
 * \brief Carga los datos del archivo de configuración.
 *
 * Esta función carga todos los datos que se encuentren en el archivo de
 * configuración con que fue instanciada la clase y los deja disponible
 * en memoria. Si el archivo no existe, la función lo creará.
 * 
 * \return true si no hubo ningún error, false en caso contrario.
 * 
 ***************************************************************************/
bool jjConfig::cargar_datos()
{
	ifstream entrada(this->file.c_str());
	if (!entrada.good())
		return false;
	string linea, clave, valor;
	//analizar cada línea separando clave de valor (clave=valor):
	while (getline(entrada, linea)){
		size_t posigual = linea.find("=");
		if (posigual != string::npos){
			clave = linea.substr(0, posigual);
			valor = linea.substr(posigual+1);
			trim(clave);
			trim(valor);
			this->data.insert(make_pair(clave, valor));
		}
	}
	entrada.close();
	return true;
}


/************************************************************************//**
 * \brief Guarda todos los datos a disco.
 *
 * Esta función guarda todas las opciones de configuración que tenga en
 * memoria la clase a disco, usando el archivo de configuración con que fue
 * instanciada la clase. Si desea hacerse permanentes todas las opciones
 * configuradas con la clase, debe llamarse a esta función manualmente para
 * persistir los datos, en ningún momento se lo hace automáticamente.
 * 
 * \return true si no hubo ningún error, false en caso contrario.
 * 
 ***************************************************************************/
bool jjConfig::Guardar()
{
	ofstream salida(this->file.c_str(), ios::trunc);
	if (!salida.good())
		return false;
	map<string, string>::iterator it = this->data.begin();
	while (it != this->data.end()){
		salida<<it->first<<"="<<it->second<<'\n';
		it++;
	}
	salida.close();
	return true;
}


/************************************************************************//**
 * \brief Configurar un valor (cadena).
 *
 * Agrega o actualiza un nuevo valor a las opciones de configuración.
 * 
 * \param Clave Nombre de la opción.
 * \param Val Valor a guardar.
 * 
 ***************************************************************************/
void jjConfig::SetValor(string Clave, string Val)
{
	this->data[Clave] = Val;
}


/************************************************************************//**
 * \brief Configurar un valor (entero).
 *
 * Agrega o actualiza un nuevo valor a las opciones de configuración.
 * 
 * \param Clave Nombre de la opción.
 * \param Val Valor a guardar.
 * 
 ***************************************************************************/
void jjConfig::SetValor(string Clave, int Val)
{
	string v = int2str(Val);
	SetValor(Clave, v);
}


/************************************************************************//**
 * \brief Configurar un valor (entero sin signo).
 *
 * Agrega o actualiza un nuevo valor a las opciones de configuración.
 * 
 * \param Clave Nombre de la opción.
 * \param Val Valor a guardar.
 * 
 ***************************************************************************/
void jjConfig::SetValor(string Clave, unsigned int Val)
{
	string v = uint2str(Val);
	SetValor(Clave, v);
}


/************************************************************************//**
 * \brief Configurar un valor (flotante).
 *
 * Agrega o actualiza un nuevo valor a las opciones de configuración.
 * 
 * \param Clave Nombre de la opción.
 * \param Val Valor a guardar.
 * 
 ***************************************************************************/
void jjConfig::SetValor(string Clave, double Val)
{
	string v = dbl2str(Val);
	SetValor(Clave, v);
}


/************************************************************************//**
 * \brief Obtener un valor (cadena).
 *
 * Obtiene el valor de una opción de configuración. Si la opción no se
 * encuentra, devuelve el valor por defecto pasado como parámetro.
 * 
 * \param Clave Nombre de la opción.
 * \param Default Valor por defecto en caso de que la opción todavía no exista.
 * \return El valor de la opción (de tipo string).
 * 
 ***************************************************************************/
string jjConfig::Valor(string Clave, string Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return it->second;
}


/************************************************************************//**
 * \brief Obtener un valor (entero).
 *
 * Obtiene el valor de una opción de configuración. Si la opción no se
 * encuentra, devuelve el valor por defecto pasado como parámetro.
 * 
 * \param Clave Nombre de la opción.
 * \param Default Valor por defecto en caso de que la opción todavía no exista.
 * \return El valor de la opción (de tipo int).
 * 
 ***************************************************************************/
int jjConfig::ValorInt(string Clave, int Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return str2int(it->second);
}


/************************************************************************//**
 * \brief Obtener un valor (entero sin signo).
 *
 * Obtiene el valor de una opción de configuración. Si la opción no se
 * encuentra, devuelve el valor por defecto pasado como parámetro.
 * 
 * \param Clave Nombre de la opción.
 * \param Default Valor por defecto en caso de que la opción todavía no exista.
 * \return El valor de la opción (de tipo unsigned int).
 * 
 ***************************************************************************/
unsigned int jjConfig::ValorUInt(string Clave, unsigned int Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return str2uint(it->second);
}


/************************************************************************//**
 * \brief Obtener un valor (flotante).
 *
 * Obtiene el valor de una opción de configuración. Si la opción no se
 * encuentra, devuelve el valor por defecto pasado como parámetro.
 * 
 * \param Clave Nombre de la opción.
 * \param Default Valor por defecto en caso de que la opción todavía no exista.
 * \return El valor de la opción (de tipo double).
 * 
 ***************************************************************************/
double jjConfig::ValorDouble(string Clave, double Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return str2dbl(it->second);
}



/****************************************************************************
 * FUNCIONES DE UTILERÍA (PRIVADAS):
 ***************************************************************************/

/************************************************************************//**
 * \brief Quita espacios de sobra a una cadena.
 *
 * trim borra los espacios y tabulaciones al inicio y final de una cadena. Se
 * puede llamar directamente trim(cadena), los cambios son reflejados en la
 * cadena sin necesidad de una nueva asignación.
 * 
 * \param Cadena Cadena por referencia, los cambios se aplican sobre ella.
 * \return Una referencia de la cadena trabajada.
 * 
 ***************************************************************************/
string& jjConfig::trim(string &Cadena)
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


/************************************************************************//**
 * \brief Cadena a entero.
 *
 * Convierte una cadena string a entero int. Si no se puede realizar la
 * conversión retorna un 0.
 * 
 * \param Cadena Cadena a convertir a entero.
 * \return La cadena convertida a int.
 * 
 ***************************************************************************/
int jjConfig::str2int(string &Cadena)
{
	return strtol(Cadena.c_str(), NULL, 10);
}


/************************************************************************//**
 * \brief Cadena a entero sin signo.
 *
 * Convierte una cadena string a entero sin signo unsigned int. Si no se
 * puede realizar la conversión retorna un 0.
 * 
 * \param Cadena Cadena a convertir a entero sin signo.
 * \return La cadena convertida a unsigned int.
 * 
 ***************************************************************************/
unsigned int jjConfig::str2uint(string &Cadena)
{
	return strtoul(Cadena.c_str(), NULL, 10);
}


/************************************************************************//**
 * \brief Cadena a flotante de doble precisión.
 *
 * Convierte una cadena string a número flotante de doble precisión double.
 * Si no se puede realizar la conversión retorna un 0.
 * 
 * \param Cadena Cadena a convertir a flotante de doble precisión.
 * \return La cadena convertida a double.
 * 
 ***************************************************************************/
double jjConfig::str2dbl(string &Cadena)
{
	return strtod(Cadena.c_str(), NULL);
}


/************************************************************************//**
 * \brief Entero a cadena.
 *
 * Convierte un entero int a cadena string.
 * 
 * \param Entero Entero a convertir en cadena.
 * \return El número convertido a string.
 * 
 ***************************************************************************/
string jjConfig::int2str(int Entero)
{
	char buf[25];
	sprintf(buf, "%d", Entero);
	string s(buf);
	return s;
}


/************************************************************************//**
 * \brief Entero sin signo a cadena.
 *
 * Convierte un entero sin signo unsigned int a cadena string.
 * 
 * \param Entero Entero sin signo a convertir en cadena.
 * \return El número convertido a string.
 * 
 ***************************************************************************/
string jjConfig::uint2str(unsigned int Entero)
{
	char buf[25];
	sprintf(buf, "%ud", Entero);
	string s(buf);
	return s;
}


/************************************************************************//**
 * \brief Flotante a cadena.
 *
 * Convierte un flotante de doble precisión double a cadena string.
 * 
 * \param Doble Flotante de doble precisión a convertir en cadena.
 * \return El número convertido a string.
 * 
 ***************************************************************************/
string jjConfig::dbl2str(double Doble)
{
	char buf[500];
	sprintf(buf, "%f", Doble);
	string s(buf);
	return s;
}
