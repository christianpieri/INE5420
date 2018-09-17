#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "Ponto.hpp"

class Window {

private:
    Ponto minimo;
    Ponto maximo;

public:
    Window();
    virtual ~Window();

    Window(const Window &origem);
    Window(double minimoX, double minimoY, double maximoX, double maximoY);

    void setCoordenadasMinimo(double x, double y);

    void setCoordenadasMaximo(double x, double y);

    double getValorXMinimo();

    double getValorXMaximo();

    double getValorYMinimo();

    double getValorYMaximo();

};

#endif	  	     	  	      	     	 	    	        	 	