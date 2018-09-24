using namespace std;
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Ponto.hpp"
#include "Reta.hpp"
#include "Poligono.hpp"
static void salvarObjetosPontoEmArquivo(std::vector<Ponto*> objetosPonto) {
    ofstream escreve;
    escreve.open("saida.obj");
        
    for(int i = 0; i < objetosPonto.size(); i ++) {
        auto ponto = objetosPonto.at(i);
        escreve << "o " << ponto->getNome() << "\n";
        escreve << "v " << ponto->getValorX() << " " << ponto->getValorY() << "\n";
    }
    escreve.close();
}
static void salvarObjetosRetaEmArquivo(std::vector<Reta*> objetosReta) {
    ofstream escreve;
    escreve.open("saida.obj", ofstream::ios_base::app);
    
    for(int i = 0; i < objetosReta.size(); i ++) {
        auto reta = objetosReta.at(i);
        escreve << "o " << reta->getNome() << "\n";
        escreve << "v " << reta->getValorXInicial() << " " << reta->getValorYInicial() << "\n";
        escreve << "v " << reta->getValorXFinal() << " " << reta->getValorYFinal() << "\n";
    }
    escreve.close();
}
static void salvarObjetosPoligonoEmArquivo(std::vector<Poligono*> objetosPoligono) {
    ofstream escreve;
    escreve.open("saida.obj", ofstream::ios_base::app);
    
    for(int i = 0; i < objetosPoligono.size(); i++) {
        auto poligono = objetosPoligono.at(i);
        escreve << "o " << poligono->getNome() << "\n";
        for(int j = 0; j < poligono->getListaDePontos().size(); j++) {
            auto ponto = poligono->getListaDePontos().at(j);
            escreve << "v " << ponto->getValorX() << " " << ponto->getValorY() << "\n";
        }
    }
    escreve.close();
}
static std::vector<Poligono*> lerObjetosDoArquivo() {
    ifstream leitura;
    leitura.open("saida.obj");
    char linhaLida[100];
    std::string nome;
    std::string lixo;
    std::string x;
    std::string y;
    std::string linha;
    std::vector<Poligono*> vetorDePoligonos;
    Poligono *poligono;
    std::vector<Ponto*> listaDePontosDoPoligono;
    
    while(leitura.getline(linhaLida, 100)) {
        linha = linhaLida;
        stringstream iss(linha);
        if(linha.substr(0, 1).compare("o") == 0) {
            listaDePontosDoPoligono.clear();
            poligono = new Poligono(listaDePontosDoPoligono, "lixo");
            iss >> lixo;
            iss >> nome;
            poligono->setNome(nome);
            vetorDePoligonos.push_back(poligono);
        } else {
            Ponto *ponto = new Ponto();
            iss >> lixo;
            iss >> x;
            iss >> y;
            ponto->setValorX(atof(x.c_str()));
            ponto->setValorY(atof(y.c_str()));
            listaDePontosDoPoligono.push_back(ponto);
            poligono->setListaDePontos(listaDePontosDoPoligono);
        }
    }
    leitura.close();
    return vetorDePoligonos;
} 