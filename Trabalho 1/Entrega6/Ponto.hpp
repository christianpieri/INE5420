#ifndef PONTO_HPP
#define PONTO_HPP
#include <string>

class Ponto {

private:
    double x;
    double y;
    std::string nome;
    
public:
    Ponto();
    virtual ~Ponto();
    
    Ponto(const Ponto  &origem);
    Ponto(double coordenadaX, double coordenadaY);
    Ponto(double coordenadaX, double coordenadaY, std::string nome);
    
    void setValorX(double coordenadaX);
    
    void setValorY(double coordenadaY);

    void setNome(std::string _nome);

    double getValorX();
    
    double getValorY();

    std::string getNome();
    
};

#endif