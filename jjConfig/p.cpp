#include <limits>
#include <iostream>
#include "jjConfig.h"

using namespace std;

int main(){
    jjConfig c("aaaa");
    c.SetValor("casa", "sarmiento");
    string s = c.Valor("casa", "ninguna");
    c.SetValor("num", 1683);
    int i = c.ValorInt("num", 5);

    //c.SetValor("float", 3.14156);

    cout<<s<<" "<<i<<" "<<c.ValorDouble("float", 0.0)<<endl;
    c.Guardar();
    
    return 0;
}
