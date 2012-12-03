#include <iostream>
#include "argparser.h"

int main(int argc, char **argv){

    ArgParserNew(p);
    p.opcion("uno", 'n', "Una opción", "5");
    p.opcion_bool("b", 'b', "Algo sin parámetros");
    
    p.parsear();
    double o = p.get_double("uno");
    cout<<"La opción pasada es: "<<o<<endl;
    if (p.is("b"))
	cout<<"Se pasó la b.\n";

    vector<const char*> s = p.get_sueltos();
    cout<<"Sueltos:\n";
    for (size_t i=0; i<s.size(); ++i){
	cout<<s[i]<<endl;
    }
    return 0;
}