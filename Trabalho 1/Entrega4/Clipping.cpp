using namespace std;
#include <cmath>
#include <iostream>
#include "Window.hpp"

static double retornaValorAuxiliar(Window *tela) {
    double distanciaEntre2Numeros = tela->getValorXMaximo() - tela->getValorXMinimo();
    double aux = (distanciaEntre2Numeros - 500) / 10;
    aux = aux + 50;
    return aux;
}

static bool devoClipparPonto(double x, double y, Window *tela) {

    double aux = retornaValorAuxiliar(tela);

    if(x > tela->getValorXMaximo() - aux || x < tela->getValorXMinimo() + aux || 
       y > tela->getValorYMaximo() - aux || y < tela->getValorYMinimo() + aux){
        return true;
    } else {
        return false;
    }
}