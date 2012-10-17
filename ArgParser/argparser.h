#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

#define ArgParserNew(x) ArgParser x(argc, argv)

class ArgParser {
private:
    int argc;
    char **argv;
    bool listo;
    int indice;
    stringstream opciones;
    stringstream usage;
    map<string, string> data;
    map<string, bool> sin_param;
    map<char, string> mapeo;
    map<char, string> mapeo_bool;
    vector<const char*> sueltos;

    void verificar_parseado();

public:
    ArgParser();
    ArgParser(int argc, char **argv) : argc(argc), argv(argv), listo(false) {}
    void opcion_bool(const char *nombre, char opcion, const char *descripcion);
    void opcion(const char *nombre, char opcion, const char *descripcion, const char *defecto);
    void parsear(bool mostrar_uso = true);
    int get(const char *nombre);
    bool is(const char *nombre);
    void mostrar_ayuda();
    vector<const char*> get_sueltos();
};

#endif /* ARGPARSER_H */ 
