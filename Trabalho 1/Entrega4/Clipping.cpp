using namespace std;
#include <cmath>
#include "Window.hpp"

static double retornaValorAuxiliar(Window *tela) {
    double aux = fabs(tela->getValorXMinimo());
    return 50 + aux;
}


static bool devoClipparPonto(double x, double y, Window *tela) {

    double distanciaEntre2Numeros = tela->getValorXMaximo() - tela->getValorXMinimo();
    // 500,        510,        490
    double aux = (distanciaEntre2Numeros - 500) / 2;

    aux = aux + 50;

    if(x > tela->getValorXMaximo() - aux || x < tela->getValorXMinimo() + aux || 
       y > tela->getValorYMaximo() - aux || y < tela->getValorYMinimo() + aux){
        return true;
    } else {
        return false;
    }
}