#include "jjConfig.h"

using namespace std;

jjConfig::jjConfig(string Archivo)
{
	this->file = Archivo;
	cargar_datos();
}

bool jjConfig::cargar_datos()
{
	ifstream entrada(this->file.c_str());
	if (!entrada.good()) //error al abrir el archivo, o no existe
		return false;
	string linea, clave, valor;
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

string jjConfig::Valor(string Clave, string Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return it->second;
}

void jjConfig::SetValor(string Clave, string Val)
{
	this->data[Clave] = Val;
}

void jjConfig::SetValor(string Clave, int Val)
{
	string v = int2str(Val);
	SetValor(Clave, v);
}

void jjConfig::SetValor(string Clave, unsigned int Val)
{
	string v = uint2str(Val);
	SetValor(Clave, v);
}

void jjConfig::SetValor(string Clave, double Val)
{
	string v = dbl2str(Val);
	SetValor(Clave, v);
}

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

int jjConfig::ValorInt(string Clave, int Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return str2int(it->second);
}

unsigned int jjConfig::ValorUInt(string Clave, unsigned int Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return str2uint(it->second);
}


double jjConfig::ValorDouble(string Clave, double Default)
{
	map<string, string>::iterator it;
	it = this->data.find(Clave);
	if (it == this->data.end())
		return Default;
	return str2dbl(it->second);
}

















//funciones de utilería -------------------------------------------------------

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

int jjConfig::str2int(string &Cadena)
{
	return strtol(Cadena.c_str(), NULL, 10);
}

unsigned int jjConfig::str2uint(string &Cadena)
{
	return strtoul(Cadena.c_str(), NULL, 10);
}

string jjConfig::int2str(int Entero)
{
	char buf[25];
	sprintf(buf, "%d", Entero);
	string s(buf);
	return s;
}

string jjConfig::uint2str(unsigned int Entero)
{
	char buf[25];
	sprintf(buf, "%ud", Entero);
	string s(buf);
	return s;
}

double jjConfig::str2dbl(string &Cadena)
{
	return strtod(Cadena.c_str(), NULL);
}

string jjConfig::dbl2str(double Doble)
{
	char buf[500];
	sprintf(buf, "%f", Doble);
	string s(buf);
	return s;
}
