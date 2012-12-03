#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include "argparser.h"

void ArgParser::opcion(const char *nombre, char opcion,
	const char *descripcion, const char *defecto)
{
	string snombre(nombre);
	string sdefault(defecto);
	this->opciones<<opcion<<":";
	this->usage<<"-"<<opcion<<" "<<nombre<<":\n\t"<<descripcion<<endl;
	this->data[snombre] = sdefault;
	this->mapeo[opcion] = snombre;
}

void ArgParser::verificar_parseado()
{
	if (!this->listo){
		cerr<<"Error: Falta llamar a la función \"ArgParser::parsear()\".\n";
		abort();
	}
}

void ArgParser::parsear(bool mostrar_uso)
{
	this->listo = true;
	//opterr = false;
	int o;
	while ((o = getopt(this->argc, this->argv, this->opciones.str().c_str())) != -1){
		if (o == '?'){
			if (mostrar_uso){
				mostrar_ayuda();
			}
			exit(-1);
		}
		map<char, string>::iterator it = this->mapeo.begin();
		while (it != this->mapeo.end()){
			if ((int)it->first == o){
				this->data[it->second] = optarg;
			}
			it++;
		}
		it = this->mapeo_bool.begin();
		while (it != this->mapeo_bool.end()){
			if ((int)it->first == o){
				this->sin_param[it->second] = true;
			}
			it++;
		}
	}
	//this->indice = optind; //índice en argv desde el cual hay parámetros sin opciones
	for (int i=optind; i<this->argc; ++i){
		this->sueltos.push_back(this->argv[i]);
	}
	
}


string& ArgParser::get(const char *nombre)
{
	verificar_parseado();
	string snombre(nombre);
	map<string, string>::iterator it = this->data.find(snombre);
	if (it == this->data.end()){
		cerr<<"Error: No fue agregada una opción con el nombre \""<<snombre<<"\".\n";
		abort();
	}
	return it->second;
}


int ArgParser::get_int(const char *nombre)
{
	int res;
	stringstream(get(nombre))>>res;
	return res;
}

unsigned int ArgParser::get_uint(const char *nombre)
{
	unsigned int res;
	stringstream(get(nombre))>>res;
	return res;
}


double ArgParser::get_double(const char *nombre)
{
	double res;
	stringstream(get(nombre))>>res;
	return res;
}


float ArgParser::get_float(const char *nombre)
{
	float res;
	stringstream(get(nombre))>>res;
	return res;
}


string ArgParser::get_string(const char *nombre)
{
	return get(nombre);
}

const char* ArgParser::get_char(const char *nombre)
{
	return get(nombre).c_str();
}


bool ArgParser::is(const char *nombre)
{
	verificar_parseado();
	string snombre(nombre);
	map<string, bool>::iterator it = this->sin_param.find(snombre);
	if (it == this->sin_param.end()){
		cerr<<"Error: No fue agregada una opción con el nombre \""<<snombre<<"\".\n";
		abort();
	}
	else {
		return it->second;
	}
}

void ArgParser::mostrar_ayuda()
{
	cout<<this->usage.str();
}


void ArgParser::opcion_bool(const char *nombre, char opcion, const char *descripcion)
{
	string snombre(nombre);
	this->opciones<<opcion;
	this->usage<<"-"<<opcion<<":\n\t"<<descripcion<<endl;
	this->sin_param[snombre] = false;
	this->mapeo_bool[opcion] = snombre;
}

vector<const char*> ArgParser::get_sueltos()
{
	verificar_parseado();
	return this->sueltos;
}
