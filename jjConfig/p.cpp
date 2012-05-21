#include <limits>
#include <iostream>
#include "jjConfig.h"

using namespace std;

int main(){
    jjConfig c("aaaa");
    c.SetValor("casa", "sarmiento");
    string s = c.Valor("casa", "ninguna");
    c.SetValor("num", "casa");
    //cout<<strtod("77.7778", NULL)<<endl;
    float i = c.ValorDouble("num", 5);

    //c.SetValor("float", 3.14156);

    cout<<s<<" "<<i<<" "<<c.ValorDouble("float", 0.0)<<endl;
    c.Guardar();
    
    return 0;
}
