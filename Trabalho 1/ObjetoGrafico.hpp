#ifndef OBJETOGRAFICO_HPP
#define OBJETOGRAFICO_HPP
#include <string>

class ObjetoGrafico {

protected:
    std::string nome;

public:
    ObjetoGrafico();

    ObjetoGrafico(const ObjetoGrafico &origem);

    virtual ~ObjetoGrafico();

    void setNomeObjetoGrafico(std::string novoNome);

    std::string getNomeObjetoGrafico();
};

#endif