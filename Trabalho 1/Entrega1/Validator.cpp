#include "Validator.hpp"

Validator::Validator() {
}

Validator::~Validator() {
}

bool Validator::validaNomeExistentePonto(std::string nome, std::vector<Ponto*> vetor) {

    bool objetoJaExistente = false;

    for (std::vector<Ponto*>::iterator it = vetor.begin(); it != vetor.end(); ++it)  {
        
        if(nome.compare((*it)->getNome()) == 0) {
            objetoJaExistente = true;
        }
    }

    return objetoJaExistente;
        
}