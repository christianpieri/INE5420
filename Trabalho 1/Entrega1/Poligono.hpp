#ifndef POLIGONO_HPP
#define POLIGONO_HPP
#include <string>
#include <vector>
#include "Ponto.hpp"

class Poligono {

private:
    std::vector<Ponto*> listaDePontos;
    std::string nome;
    
public:
    Poligono();
    virtual ~Poligono();
    
    Poligono(const Poligono  &origem);
    Poligono(std::vector<Ponto*> listaDePontos, std::string nome);
    
    void setListaDePontos(std::vector<Ponto*> _pontos);
    std::vector<Ponto*> getListaDePontos();

    void setNome(std::string _nome);
    std::string getNome();
    
};

#endif