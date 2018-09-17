#ifndef RETA_HPP
#define RETA_HPP
#include <string>

class Reta {

private:
    double xInicial;
    double xFinal;
    double yInicial;
    double yFinal;
    std::string nome;
    
public:
    Reta();
    virtual ~Reta();
    
    Reta(const Reta  &origem);
    Reta(double coordenadaXInicial, double coordenadaYInicial, double coordenadaXFinal, double coordenadaYFinal, std::string nome);
    
    void setValorXInicial(double coordenadaXInicial);
    void setValorXFinal(double coordenadaXFinal);
    void setValorYInicial(double coordenadaYInicial);
    void setValorYFinal(double coordenadaYFinal);

    double getValorXInicial();
    double getValorXFinal();
    double getValorYInicial();
    double getValorYFinal();

    void setNome(std::string _nome);
    std::string getNome();
    
};

#endif