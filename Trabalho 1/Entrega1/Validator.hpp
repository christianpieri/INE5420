#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP
#include <string>
#include <vector>
#include "Ponto.hpp"

class Validator {

private:    
public:
    Validator();
    virtual ~Validator();
    
    bool validaNomeExistentePonto(std::string nome, std::vector<Ponto*> vetor);

};

#endif