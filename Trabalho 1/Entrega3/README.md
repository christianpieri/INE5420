Para compilar use:

gcc `pkg-config --cflags gtk+-3.0` -o vpl_wexecution main.cpp Window.cpp Ponto.cpp Reta.cpp Poligono.cpp SaveLoadObj.cpp RotacaoWindow.cpp `pkg-config --libs gtk+-3.0` -rdynamic -lstdc++ -std=c++11 -lm

Para rodar use:

./vpl_wexecution
