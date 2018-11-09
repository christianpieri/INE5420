#include "Reta.hpp"

Reta::Reta() {
}

Reta::Reta(const Reta &origem) {
}

Reta::~Reta() {
}

Reta::Reta(double coordenadaXInicial, double coordenadaYInicial, double coordenadaXFinal, double coordenadaYFinal, std::string _nome) {
    xInicial = coordenadaXInicial;
    xFinal = coordenadaXFinal;
    yInicial = coordenadaYInicial;
    yFinal = coordenadaYFinal;
    nome = _nome;
}


void Reta::setValorXInicial(double coordenadaXInicial) {
    xInicial = coordenadaXInicial;
}

void Reta::setValorXFinal(double coordenadaXFinal) {
    xFinal = coordenadaXFinal;
}

void Reta::setValorYInicial(double coordenadaYInicial) {
    yInicial = coordenadaYInicial;
}
        
void Reta::setValorYFinal(double coordenadaYFinal) {
    yFinal = coordenadaYFinal;
}

double Reta::getValorXInicial() {
    return xInicial;
}

double Reta::getValorXFinal() {
    return xFinal;
}
    
double Reta::getValorYInicial() {
    return yInicial;
}

double Reta::getValorYFinal() {
    return yFinal;
}

void Reta::setNome(std::string _nome) {
     nome = _nome;
}

std::string Reta::getNome() {
    return nome;
}