#include "Window.hpp"

Window::Window() {
}

Window::Window(const Window &origem) {
}

Window::~Window() {
}

Window::Window(double minimoX, double minimoY, double maximoX, double maximoY) {
    minimo = Ponto(minimoX, minimoY);
    maximo = Ponto(maximoX, maximoY);
}

void Window::setCoordenadasMinimo(double x, double y) {
    minimo.setValorX(x);
    minimo.setValorY(y);
}

void Window::setCoordenadasMaximo(double x, double y) {
    maximo.setValorX(x);
    maximo.setValorY(y);
}

double Window::getValorXMinimo() {
    return minimo.getValorX();
}

double Window::getValorXMaximo() {
    return maximo.getValorX();
}

double Window::getValorYMinimo() {
    return minimo.getValorY();
}

double Window::getValorYMaximo() {
    return maximo.getValorY();
}