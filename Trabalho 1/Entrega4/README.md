Para compilar use:

gcc `pkg-config --cflags gtk+-3.0` -o vpl_wexecution main.cpp Window.cpp Ponto.cpp Reta.cpp Poligono.cpp Curva.cpp SaveLoadObj.cpp RotacaoWindow.cpp Clipping.cpp `pkg-config --libs gtk+-3.0` -rdynamic -lstdc++ -std=c++11 -lm

Para rodar use:

./vpl_wexecution


** Lembrando que **
A entrega 4 foi feita depois da entrega 6, portanto é a mais recente!
