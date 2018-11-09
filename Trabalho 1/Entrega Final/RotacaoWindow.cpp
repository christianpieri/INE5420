using namespace std;
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Ponto.hpp"
#include "Reta.hpp"
#include "Poligono.hpp"
#include "Curva.hpp"
#include "Window.hpp"
#include <cmath>
#define PI 3.14159265

static std::vector<Ponto*> rotacionaTodosOsPontos(std::string sentido,std::vector<Ponto*> listaDePontos, Window *tela) {
    double angulo = 90 * PI / 180;
    if(sentido.compare("Direita") == 0) {
        angulo = angulo * (-1);
    }

    std::vector<Ponto*> novaListaDePontos;

    double x = (tela->getValorXMinimo() + tela->getValorXMaximo()) / 2;
    double y = (tela->getValorYMinimo() + tela->getValorYMaximo()) / 2;

    for (std::vector<Ponto*>::iterator it = listaDePontos.begin(); it != listaDePontos.end(); ++it) {
        double novoX = ((*it)->getValorX() - x) * cos(angulo) + ((*it)->getValorY() - y) * sin(angulo) + x;
        double novoY = ((*it)->getValorX() - x) * -sin(angulo) + ((*it)->getValorY() - y) * cos(angulo) + y;
        (*it)->setValorX(novoX);
        (*it)->setValorY(novoY);
        novaListaDePontos.push_back((*it));
    }

    return novaListaDePontos;
}

static std::vector<Reta*> rotacionaTodasAsRetas(std::string sentido, std::vector<Reta*> listaDeRetas, Window *tela) {
    double angulo = 90 * PI / 180;
    if(sentido.compare("Direita") == 0) {
        angulo = angulo * (-1);
    }

    std::vector<Reta*> novaListaDeRetas;

    double x = (tela->getValorXMinimo() + tela->getValorXMaximo()) / 2;
    double y = (tela->getValorYMinimo() + tela->getValorYMaximo()) / 2;
    
    for (std::vector<Reta*>::iterator it = listaDeRetas.begin(); it != listaDeRetas.end(); ++it) {
        double novoXInicial = ((*it)->getValorXInicial() - x) * cos(angulo) + ((*it)->getValorYInicial() - y) * sin(angulo) + x;
        double novoYInicial = ((*it)->getValorXInicial() - x) * -sin(angulo) + ((*it)->getValorYInicial() - y) * cos(angulo) + y;
        double novoXFinal = ((*it)->getValorXFinal() - x) * cos(angulo) + ((*it)->getValorYFinal() - y) * sin(angulo) + x;
        double novoYFinal = ((*it)->getValorXFinal() - x) * -sin(angulo) + ((*it)->getValorYFinal() - y) * cos(angulo) + y;
        
        (*it)->setValorXInicial(novoXInicial);
        (*it)->setValorYInicial(novoYInicial);
        (*it)->setValorXFinal(novoXFinal);
        (*it)->setValorYFinal(novoYFinal);

        novaListaDeRetas.push_back((*it));
    }
    
    return novaListaDeRetas;
}

static std::vector<Poligono*> rotacionaTodosOsPoligonos(std::string sentido, std::vector<Poligono*> listaDePoligonos, Window *tela) {
    double angulo = 90 * PI / 180;
    if(sentido.compare("Direita") == 0) {
        angulo = angulo * (-1);
    }

    std::vector<Poligono*> novaListaDePoligonos;
    
    double x = (tela->getValorXMinimo() + tela->getValorXMaximo()) / 2;
    double y = (tela->getValorYMinimo() + tela->getValorYMaximo()) / 2;

    for (std::vector<Poligono*>::iterator it = listaDePoligonos.begin(); it != listaDePoligonos.end(); ++it) {
        for(int i = 0; i < (*it)->getListaDePontos().size(); i++) {
            auto ponto = (*it)->getListaDePontos().at(i);
            double novoX = (ponto->getValorX() - x) * cos(angulo) + (ponto->getValorY() - y) * sin(angulo) + x;
            double novoY = (ponto->getValorX() - x) * -sin(angulo) + (ponto->getValorY() - y) * cos(angulo) + y;
            ponto->setValorX(novoX);
            ponto->setValorY(novoY);
        }

        novaListaDePoligonos.push_back((*it));
    }
    
    return novaListaDePoligonos;
}

static std::vector<Curva*> rotacionaTodasAsCurvas(std::string sentido, std::vector<Curva*> listaDeCurvas, Window *tela) {
    double angulo = 90 * PI / 180;
    if(sentido.compare("Direita") == 0) {
        angulo = angulo * (-1);
    }

    std::vector<Curva*> novaListaDeCurvas;
    
    double x = (tela->getValorXMinimo() + tela->getValorXMaximo()) / 2;
    double y = (tela->getValorYMinimo() + tela->getValorYMaximo()) / 2;

    for (std::vector<Curva*>::iterator it = listaDeCurvas.begin(); it != listaDeCurvas.end(); ++it) {
        for(int i = 0; i < (*it)->getListaDePontos().size(); i++) {
            auto ponto = (*it)->getListaDePontos().at(i);
            double novoX = (ponto->getValorX() - x) * cos(angulo) + (ponto->getValorY() - y) * sin(angulo) + x;
            double novoY = (ponto->getValorX() - x) * -sin(angulo) + (ponto->getValorY() - y) * cos(angulo) + y;
            ponto->setValorX(novoX);
            ponto->setValorY(novoY);
        }

        novaListaDeCurvas.push_back((*it));
    }
    
    return novaListaDeCurvas;
}