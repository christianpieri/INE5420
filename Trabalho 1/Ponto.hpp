#ifndef PONTO_HPP
#define PONTO_HPP
#include "ObjetoGrafico.hpp"
#include <string>

class Ponto : public ObjetoGrafico {

private:
    double x;
    double y;
    
public:
    Ponto();
    virtual ~Ponto();
    
    Ponto(const Ponto  &origem);
    Ponto(double coordenadaX, double coordenadaY);
    Ponto(double coordenadaX, double coordenadaY, std::string nome);
    
    void setValorX(double coordenadaX);
    
    void setValorY(double coordenadaY);

    double getValorX();
    
    double getValorY();
    
};

#endif