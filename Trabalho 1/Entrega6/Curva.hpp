#ifndef CURVA_HPP
#define CURVA_HPP
#include <string>
#include <vector>
#include "Ponto.hpp"

class Curva {

private:
    std::vector<Ponto*> listaDePontos;
    std::string nome;
    
public:
    Curva();
    virtual ~Curva();
    
    Curva(const Curva  &origem);
    Curva(std::vector<Ponto*> listaDePontos, std::string nome);
    
    void setListaDePontos(std::vector<Ponto*> _pontos);
    std::vector<Ponto*> getListaDePontos();

    void setNome(std::string _nome);
    std::string getNome();
    
};

#endif