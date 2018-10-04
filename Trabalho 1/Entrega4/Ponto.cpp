#include "Ponto.hpp"

Ponto::Ponto() {
}

Ponto::Ponto(const Ponto &origem) {
}

Ponto::~Ponto() {
}

Ponto::Ponto(double coordenadaX, double coordenadaY) {
    x = coordenadaX;
    y = coordenadaY;
}

Ponto::Ponto(double coordenadaX, double coordenadaY, std::string _nome) {
    x = coordenadaX;
    y = coordenadaY;
    nome = _nome;
}

void Ponto::setValorX(double coordenadaX) {
    x = coordenadaX;
}

void Ponto::setValorY(double coordenadaY) {
    y = coordenadaY;
}                                                                   

double Ponto::getValorX() {
    return x;
}

double Ponto::getValorY() {
    return y;
}

std::string Ponto::getNome() {
    return nome;
}