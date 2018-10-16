using namespace std;
#include <gtk/gtk.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "Window.hpp"

static double retornaValorAuxiliar(Window *tela) {
    double distanciaEntre2Numeros = tela->getValorXMaximo() - tela->getValorXMinimo();
    double aux = (distanciaEntre2Numeros - 500) / 10;
    aux = aux + 50;
    return aux;
}

static bool devoClipparPonto(double x, double y, Window *tela) {

    double aux = retornaValorAuxiliar(tela);

    if(x > tela->getValorXMaximo() - aux || x < tela->getValorXMinimo() + aux || 
       y > tela->getValorYMaximo() - aux || y < tela->getValorYMinimo() + aux){
        return true;
    } else {
        return false;
    }
}

static std::vector<double> liangBarskyClippingLine(double x0, double y0, double x1, double y1, Window *tela) {
    	
        std::vector<double> pontos;
        double aux = retornaValorAuxiliar(tela);
        
        double xmin = tela->getValorXMinimo() + aux;
		double xmax = tela->getValorXMaximo() - aux;
		double ymin = tela->getValorYMinimo() + aux;
		double ymax = tela->getValorYMaximo() - aux;
		
        double u1 = 0.0;
	    double u2 = 1.0;
	    double dx = x1 - x0;
	    double dy = y1 - y0;
	    double p, q, r;
	    bool draw = true;

	    for(int edge = 0; edge < 4; edge++) {
	        if (edge == 0) {
	        	p = -dx;
	        	q = x0 - xmin;
	        }
	        if (edge == 1) {
	        	p = dx;
	        	q = xmax - x0;
	        }
	        if (edge == 2) {
	        	p = -dy;
	        	q = y0 - ymin;
	        }
	        if (edge == 3) {
	        	p = dy;
	        	q = ymax - y0;
	        }   

	        r = q / p;

	        if(p == 0 && q < 0) {
	        	draw = false;   
	        }

	        if(p < 0) {
	            if(r > u2) {
	            	draw = false;
	            }
	            else if (r > u1) {
	            	u1 = r;
	            }
	        } else if(p > 0) {
	            if(r < u1) {
	            	draw = false;
	            }
	            else if(r < u2) {
	            	u2 = r;
	            }
	        }
	    }

	    if (draw) {
		    x0 = x0 + u1 * dx;
		    y0 = y0 + u1 * dy;
		    
            if (u2 != 1) {
			    x1 = x0 + u2 * dx;
			    y1 = y0 + u2 * dy;   	
		    }
            pontos.push_back(x0);
            pontos.push_back(y0);
            pontos.push_back(x1);
            pontos.push_back(y1);

		}

		return pontos;
}
