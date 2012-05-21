#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <vector>

using namespace std;

string cmd_output(const char cmd[])
{
	//http://stackoverflow.com/a/478960
	//por waqas
	FILE* pipe = popen(cmd, "r");
	if (!pipe) return "ERROR";
	char buffer[128];
	string result = "";
	while (!feof(pipe)){
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}

string human_readable(unsigned int bytes)
{
	//basado en http://bazaar.launchpad.net/~vicox/syspeek/trunk/view/head:/syspeek/helper.py
	
	float bytesf = bytes * 1.0;

	//máximo 4294967295 bytes = 4 GiB (por el unsigned int). Con eso debería
	//alcanzar para los archivos de Ubuntu One
	const char *rep[] = {"bytes", "KiB", "MiB", "GiB"};
	int repcount = 4;
	
	stringstream ss;
	for (int i=0; i<repcount; ++i){
		if (bytesf < 1024.0){
			ss<<setprecision(3)<<bytesf<<" "<<rep[i];
			return ss.str();
		}
		bytesf /= 1024.0;
	}
	return "";
}

vector<string> parsear(string &datos, const char *what)
{
	stringstream data(datos);
	string l;
	getline(data, l);
	vector<string> v;
	
	if (what == "Descargas"){
		if (l == "Current downloads: 0")
			return v;
	}
	else if (what == "Subidas"){
		if (l == "Current downloads: 0")
			return v;
	}

	stringstream res;
	while (getline(data, l)){
		string archivo = l.substr(l.find(":") + 2);
		res<<" - "<<archivo<<endl;
		getline(data, l);
		stringstream s_bytestotal(l.substr(l.find(":") + 1));
		unsigned int bytestotal; s_bytestotal>>bytestotal;
		getline(data, l);
		stringstream s_bytes(l.substr(l.find(":") + 1));
		unsigned int bytes; s_bytes>>bytes;
		float porc = bytes*100.0/bytestotal;
		res<<"   "<<human_readable(bytes)<<" de "<<human_readable(bytestotal)<<" ("<<setprecision(3)<<porc<<"%)\n";
		v.push_back(res.str());
	}

	return v;
}

string parsear_estado(string &s)
{
	stringstream data(s);
	string l, res = "";
	while (getline(data, l)){
		if (l.find("is_connected:") != string::npos){
			res += "Conexión: ";
			if (l.find("True") != string::npos)
				res += "Conectado\n";
			else
				res += "No conectado\n";
		}
		else if (l.find("queues:") != string::npos){
			res += "Estado: ";
			if (l.find("WORKING") != string::npos)
				res += "Trabajando\n";
			else if (l.find("IDLE") != string::npos)
				res += "Sincronizado\n";
		}
	}
	return res;
}

void error_salir()
{
	cout<<"Error ejecutando u1sdtool. El cliente de Ubuntu One puede no\
			estar instalado.\n";
	exit(1);
}

int main(){

	string out = cmd_output("u1sdtool --status 2>&1");
	if (out == "ERROR"){
		error_salir();
	}
	cout<<parsear_estado(out)<<endl;
	
	out = cmd_output("u1sdtool --current-transfers 2>&1");
	if (out == "ERROR"){
		error_salir();
	}
	
	int a = out.find("Current uploads:");
	int b = out.find("Current downloads:", a);

	string s = out.substr(a, b-a);
	vector<string> ups = parsear(s, "Subidas");
	cout<<"Subidas";
	if (ups.empty())
		cout<<":\nNo hay subidas actualmente.\n";
	else {
		cout<<" ("<<ups.size()<<"):\n";
		for (int i=0; i<ups.size(); ++i){
			cout<<ups[i];
		}
	}

	s = out.substr(b);
	vector<string> downs = parsear(s, "Descargas");
	cout<<"\nDescargas";
	if (downs.empty())
		cout<<":\nNo hay descargas actualmente.\n";
	else {
		cout<<" ("<<downs.size()<<"):\n";
		for (int i=0; i<downs.size(); ++i){
			cout<<downs[i];
		}
	}

	return 0;
	
}
