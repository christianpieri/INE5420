#include "Poligono.hpp"

Poligono::Poligono() {
}

Poligono::Poligono(const Poligono &origem) {
}

Poligono::~Poligono() {
}

Poligono::Poligono(std::vector<Ponto> _listaDePontos, std::string _nome) {
    listaDePontos = _listaDePontos;
    nome = _nome;
}

void Poligono::setListaDePontos(std::vector<Ponto> _pontos) {
    listaDePontos = _pontos;
}

std::vector<Ponto> Poligono::getListaDePontos() {
    return listaDePontos;
}

void Poligono::setNome(std::string _nome) {
    nome = _nome;
}

std::string Poligono::getNome() {
    return nome;
}