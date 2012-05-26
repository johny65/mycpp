
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
	

	///Carga los datos del archivo de configuración.
	/**
	 * Esta función carga todos los datos que se encuentren en el archivo de
	 * configuración con que fue instanciada la clase y los deja disponible
	 * en memoria. Si el archivo no existe, la función lo creará.
	 * 
	 * \return true si no hubo ningún error, false en caso contrario.
	 */
	bool cargar_datos();
	
	/* funciones de utilería: */
	
	///Quita espacios de sobra a una cadena.
	/**
	 * trim borra los espacios y tabulaciones al inicio y final de una cadena.
	 * Se puede llamar directamente trim(cadena), los cambios son reflejados en
	 * la cadena sin necesidad de una nueva asignación.
	 * 
	 * \param Cadena Cadena por referencia, los cambios se aplican sobre ella.
	 * \return Una referencia de la cadena trabajada.
	 */
	string& trim(string &Cadena);


	///Cadena a entero.
	/**
	 * Convierte una cadena string a entero int. Si no se puede realizar la
	 * conversión retorna un 0.
	 * 
	 * \param Cadena Cadena a convertir a entero.
	 * \return La cadena convertida a int.
	 */
	int str2int(string &Cadena);


	///Cadena a entero sin signo.
	/**
	 * Convierte una cadena string a entero sin signo unsigned int. Si no se
	 * puede realizar la conversión retorna un 0.
	 * 
	 * \param Cadena Cadena a convertir a entero sin signo.
	 * \return La cadena convertida a unsigned int.
	 */
	unsigned int str2uint(string &Cadena);


	///Cadena a flotante de doble precisión.
	/**
	 * Convierte una cadena string a número flotante de doble precisión double.
	 * Si no se puede realizar la conversión retorna un 0.
	 * 
	 * \param Cadena Cadena a convertir a flotante de doble precisión.
	 * \return La cadena convertida a double.
	 */
	double str2dbl(string &Cadena);


	///Entero a cadena.
	/**
	 * Convierte un entero int a cadena string.
	 * 
	 * \param Entero Entero a convertir en cadena.
	 * \return El número convertido a string.
	 */
	string int2str(int Entero);


	///Entero sin signo a cadena.
	/**
	 * Convierte un entero sin signo unsigned int a cadena string.
	 * 
	 * \param Entero Entero sin signo a convertir en cadena.
	 * \return El número convertido a string.
	 */
	string uint2str(unsigned int Entero);


	///Flotante a cadena.
	/**
	 * Convierte un flotante de doble precisión double a cadena string.
	 * 
	 * \param Doble Flotante de doble precisión a convertir en cadena.
	 * \return El número convertido a string.
	 */
	string dbl2str(double Doble);
	
public:

	///Constructor.
	/**
	 * Constructor de la clase. Recibe como parámetro la ruta en disco del
	 * archivo de configuración que se usará. Carga todos los datos contenidos
	 * en el archivo. Puede pasarse un archivo vacío o inexistente, en tales
	 * casos no cargará ningún dato pero al momento de guardar creará los datos
	 * necesarios.
	 * 
	 * \param Archivo Ruta del archivo de configuración.
	 */
	jjConfig(string Archivo);


	///Guarda todos los datos a disco.
	/**
	 * Esta función guarda todas las opciones de configuración que tenga en
	 * memoria la clase a disco, usando el archivo de configuración con que fue
	 * instanciada la clase. Si desea hacerse permanentes todas las opciones
	 * configuradas con la clase, debe llamarse a esta función manualmente para
	 * persistir los datos, en ningún momento se lo hace automáticamente.
	 * 
	 * \return true si no hubo ningún error, false en caso contrario.
	 */
	bool Guardar();


	/* setters: */

	///Configurar un valor (cadena).
	/**
	 * Agrega o actualiza un nuevo valor a las opciones de configuración.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Val Valor a guardar.
	 */
	void SetValor(string Clave, string Val);
	

	///Configurar un valor (entero).
	/**
	 * Agrega o actualiza un nuevo valor a las opciones de configuración.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Val Valor a guardar.
	 */
	void SetValor(string Clave, int Val);

	
	///Configurar un valor (entero sin signo).
	/**
	 * Agrega o actualiza un nuevo valor a las opciones de configuración.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Val Valor a guardar.
	 */
	void SetValor(string Clave, unsigned int Val);


	///Configurar un valor (flotante).
	/**
	 * Agrega o actualiza un nuevo valor a las opciones de configuración.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Val Valor a guardar.
	 */
	void SetValor(string Clave, double Val);

	
	/* getters: */

	///Obtener un valor (cadena).
	/**
	 * Obtiene el valor de una opción de configuración. Si la opción no se
	 * encuentra, devuelve el valor por defecto pasado como parámetro.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Default Valor por defecto en caso de que la opción todavía no exista.
	 * \return El valor de la opción (de tipo string).
	 */
	string Valor(string Clave, string Default);


	///Obtener un valor (entero).
	/**
	 * Obtiene el valor de una opción de configuración. Si la opción no se
	 * encuentra, devuelve el valor por defecto pasado como parámetro.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Default Valor por defecto en caso de que la opción todavía no exista.
	 * \return El valor de la opción (de tipo int).
	 */
	int ValorInt(string Clave, int Default);


	///Obtener un valor (entero sin signo).
	/**
	 * Obtiene el valor de una opción de configuración. Si la opción no se
	 * encuentra, devuelve el valor por defecto pasado como parámetro.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Default Valor por defecto en caso de que la opción todavía no exista.
	 * \return El valor de la opción (de tipo unsigned int).
	 */
	unsigned int ValorUInt(string Clave, unsigned int Default);


	///Obtener un valor (flotante).
	/**
	 * Obtiene el valor de una opción de configuración. Si la opción no se
	 * encuentra, devuelve el valor por defecto pasado como parámetro.
	 * 
	 * \param Clave Nombre de la opción.
	 * \param Default Valor por defecto en caso de que la opción todavía no exista.
	 * \return El valor de la opción (de tipo double).
	 */
	double ValorDouble(string Clave, double Default);
	
};

#endif
