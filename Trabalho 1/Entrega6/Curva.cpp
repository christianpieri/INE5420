#include "Curva.hpp"

Curva::Curva() {
}

Curva::Curva(const Curva &origem) {
}

Curva::~Curva() {
}

Curva::Curva(std::vector<Ponto*> _listaDePontos, std::string _nome) {
    listaDePontos = _listaDePontos;
    nome = _nome;
}

void Curva::setListaDePontos(std::vector<Ponto*> _pontos) {
    listaDePontos = _pontos;
}

std::vector<Ponto*> Curva::getListaDePontos() {
    return listaDePontos;
}

void Curva::setNome(std::string _nome) {
    nome = _nome;
}

std::string Curva::getNome() {
    return nome;
}