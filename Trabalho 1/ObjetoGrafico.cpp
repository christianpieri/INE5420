#include "ObjetoGrafico.hpp"

ObjetoGrafico::ObjetoGrafico() {
}

ObjetoGrafico::ObjetoGrafico(const ObjetoGrafico &origem) {
}

ObjetoGrafico::~ObjetoGrafico() {
}

void ObjetoGrafico::setNomeObjetoGrafico(std::string novoNome) {
    nome = novoNome;
}

std::string ObjetoGrafico::getNomeObjetoGrafico() {
    return nome;
} 	  	     	  	      	     	 	    	        	 	