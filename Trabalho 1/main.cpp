using namespace std;
#include <gtk/gtk.h>
#include <iostream>
#include "Window.hpp"
#include "Ponto.hpp"
#include "Reta.hpp"
#include "Poligono.hpp"
#include <sstream>
#include <vector>
#include <iterator>

    GtkWidget *windowPrincipal;
    GtkWidget *windowPonto;
    GtkWidget *windowReta;
    GtkWidget *windowPoligono;
    Window     tela;

    // Botões da caixa de controle
    GtkWidget *buttonZoomIn;
    GtkWidget *buttonZoomOut;
    GtkWidget *buttonCima;
    GtkWidget *buttonBaixo;
    GtkWidget *buttonEsquerda;
    GtkWidget *buttonDireita;
    GtkWidget *buttonLimparTela;

    // Botões dos objetos a serem desenhados
    GtkWidget *buttonPonto;
    GtkWidget *buttonReta;
    GtkWidget *buttonPoligono;

    // Botões da window ponto
    GtkWidget *buttonSalvarPoint;
    GtkWidget *buttonCancelarPoint;
    GtkWidget *spinPontoX;
    GtkWidget *spinPontoY;
    GtkWidget *spinPontoZ;
    GtkWidget *textEntryPointName;

    // Botões da window reta
    GtkWidget *buttonSalvarReta;
    GtkWidget *buttonCancelarReta;
    GtkWidget *spinRetaX1;
    GtkWidget *spinRetaY1;
    GtkWidget *spinRetaZ1;
    GtkWidget *spinRetaX2;
    GtkWidget *spinRetaY2;
    GtkWidget *spinRetaZ2;
    GtkWidget *textEntryRetaName;

    // Botões da window poligono
    GtkWidget *buttonSalvarPoligono;
    GtkWidget *buttonCancelarPoligono;
    GtkWidget *buttonAddPontoAoPoligono;
    GtkWidget *spinPoligonoX;
    GtkWidget *spinPoligonoY;
    GtkWidget *spinPoligonoZ;
    GtkWidget *textEntryPoligonoName;
    
    // Área de desenho
    GtkWidget *drawingArea;
    
    // Console e objetos
    GtkWidget *textConsole;
    GtkWidget *objectsList;
    GtkScrolledWindow *scrolledConsole;
    
    // Lista de Objetos
    std::vector<Ponto*> objetosPonto;
    std::vector<Reta*> objetosReta;
    std::vector<Poligono*> objetosPoligono;
    std::vector<Ponto*> pontosAuxiliarPoligono;

    // Surface
    static cairo_surface_t *surface = NULL;


// Transformada de ViewPort de X
static double transformadaViewPortCoordenadaX(double x) {
    double auxiliar = (x - tela.getValorXMinimo()) / (tela.getValorXMaximo() - tela.getValorXMinimo());

    return auxiliar * (500 - 0);
}

// Transformada de ViewPort de Y
static double transformadaViewPortCoordenadaY(double y) {   
    double auxiliar = (y - tela.getValorYMinimo()) / (tela.getValorYMaximo() - tela.getValorYMinimo());

    return (1 - auxiliar) * (500 - 0);
}

// Limpa a tela toda
static void clear_surface () {
  
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);
  cairo_destroy (cr);
 
}

// Desenha os objetos após passar pelas transformadas
static void desenharLinha(double x1, double y1, double x2, double y2) {
    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, 5);
    cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); 
    cairo_move_to (cr, x1, y1);
    cairo_line_to (cr, x2, y2);
    cairo_stroke (cr);    
    gtk_widget_queue_draw (windowPrincipal);
}

// Redesenha pontos
static void redesenhaPontos() {

    double x;
    double y;

    for (std::vector<Ponto*>::iterator it = objetosPonto.begin(); it != objetosPonto.end(); ++it) {
        x = (*it)->getValorX();
        y = (*it)->getValorY();

        desenharLinha(transformadaViewPortCoordenadaX(x),
                      transformadaViewPortCoordenadaY(y),
                      transformadaViewPortCoordenadaX(x),
                      transformadaViewPortCoordenadaY(y));
    }

}

// Redesenha retas
static void redesenhaRetas() {
    double x1;
    double y1;
    double x2;
    double y2;

    for (std::vector<Reta*>::iterator it = objetosReta.begin(); it != objetosReta.end(); ++it) {
        x1 = (*it)->getValorXInicial();
        y1 = (*it)->getValorYInicial();
        x2 = (*it)->getValorXFinal();
        y2 = (*it)->getValorYFinal();


        desenharLinha(transformadaViewPortCoordenadaX(x1),
                      transformadaViewPortCoordenadaY(y1),
                      transformadaViewPortCoordenadaX(x2),
                      transformadaViewPortCoordenadaY(y2));

    }
}

// Redesenha poligonos
static void redesenhaPoligonos() {
    double x;
    double y;

    for (std::vector<Poligono*>::iterator it = objetosPoligono.begin(); it != objetosPoligono.end(); ++it) {

        auto listaDePontos = (*it)->getListaDePontos();

        auto ponto = listaDePontos.at(0);
        
        cairo_t *cr;
        cr = cairo_create (surface);
        cairo_set_line_width (cr, 5);
        cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); 
        cairo_move_to (cr, transformadaViewPortCoordenadaX(ponto->getValorX()),
                        transformadaViewPortCoordenadaY(ponto->getValorY()));

        for (std::vector<Ponto*>::iterator it = listaDePontos.begin(); it != listaDePontos.end(); ++it) {
            x = (*it)->getValorX();
            y = (*it)->getValorY();
            cairo_line_to (cr, transformadaViewPortCoordenadaX(x), transformadaViewPortCoordenadaY(y));
        }  

        cairo_line_to (cr, transformadaViewPortCoordenadaX(ponto->getValorX()),
                        transformadaViewPortCoordenadaY(ponto->getValorY()));
    
        cairo_stroke (cr);    
    }

    gtk_widget_queue_draw (windowPrincipal);
}

// Redesenha tudo
static void reDrawAll () {
    clear_surface();

    redesenhaPontos();
    redesenhaRetas();
    redesenhaPoligonos();
  
}

// chama este método quando o botão limpar tela é clicado
static void on_buttonLimparTela_clicked() {
  
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão limpar tela pressionado!\n", -1);

    clear_surface();
    gtk_widget_queue_draw (windowPrincipal);

}

// chama este método quando o botão baixo é clicado
static void on_buttonBaixo_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão baixo pressionado! Movendo-se +10.\n", -1);

    double xMaximo = tela.getValorXMaximo();
    double xMinimo = tela.getValorXMinimo();
    double yMaximo = tela.getValorYMaximo();
    double yMinimo = tela.getValorYMinimo();

    yMaximo -= 10;
    yMinimo -= 10;
    
    tela.setCoordenadasMaximo(xMaximo,yMaximo);
    tela.setCoordenadasMinimo(xMinimo,yMinimo);
    
    reDrawAll();

}

// chama este método quando o botão cima é clicado
static void on_buttonCima_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão cima pressionado! Movendo-se +10.\n", -1);

    double xMaximo = tela.getValorXMaximo();
    double xMinimo = tela.getValorXMinimo();
    double yMaximo = tela.getValorYMaximo();
    double yMinimo = tela.getValorYMinimo();

    yMaximo += 10;
    yMinimo += 10;
    
    tela.setCoordenadasMaximo(xMaximo,yMaximo);
    tela.setCoordenadasMinimo(xMinimo,yMinimo);
    
    reDrawAll();
}

// chama este método quando o botão esquerda é clicado
static void on_buttonEsquerda_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão esquerda pressionado! Movendo-se +10.\n", -1);

    double xMaximo = tela.getValorXMaximo();
    double xMinimo = tela.getValorXMinimo();
    double yMaximo = tela.getValorYMaximo();
    double yMinimo = tela.getValorYMinimo();

    xMaximo -= 10;
    xMinimo -= 10;
    
    tela.setCoordenadasMaximo(xMaximo,yMaximo);
    tela.setCoordenadasMinimo(xMinimo,yMinimo);
    
    reDrawAll();
}

// chama este método quando o botão direita é clicado
static void on_buttonDireita_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão direita pressionado! Movendo-se +10.\n", -1);

    double xMaximo = tela.getValorXMaximo();
    double xMinimo = tela.getValorXMinimo();
    double yMaximo = tela.getValorYMaximo();
    double yMinimo = tela.getValorYMinimo();

    xMaximo += 10;
    xMinimo += 10;
    
    tela.setCoordenadasMaximo(xMaximo,yMaximo);
    tela.setCoordenadasMinimo(xMinimo,yMinimo);
    
    reDrawAll();
}

// chama este método quando o botão zoomIn é clicado
static void on_buttonZoomIn_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão zoom in pressionado!\n", -1);

}

// chama este método quando o botão zoomOut é clicado
static void on_buttonZoomOut_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão zoom out pressionado!\n", -1);
}

// chama este método quando o botão reta é clicado
static void on_buttonReta_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão reta pressionado!\n", -1);

    gtk_widget_show(windowReta);

}

// chama este método quando o botão poligono é clicado
static void on_buttonPoligono_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão poligono pressionado!\n", -1);

    gtk_widget_show(windowPoligono);
    
}

// chama este método quando o botão ponto é clicado
static void on_buttonPonto_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão ponto pressionado!\n", -1);

    gtk_widget_show(windowPonto);

}

// chama este método quando o botão salvar da window ponto é clicado
static void on_buttonSalvarPoint_clicked() {

    gtk_widget_hide(windowPonto);

    double x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPontoX));
    double y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPontoY));
    double z = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPontoZ));

    string nome = gtk_entry_get_text(GTK_ENTRY(textEntryPointName));

    desenharLinha(transformadaViewPortCoordenadaX(x), 
                  transformadaViewPortCoordenadaY(y), 
                  transformadaViewPortCoordenadaX(x), 
                  transformadaViewPortCoordenadaY(y));

    std::ostringstream console;
    console << "O ponto " << nome << "(" << x << ", " << y << ") foi desenhado." << std::endl;
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);

    Ponto *ponto = new Ponto(x, y, nome);
    objetosPonto.push_back(ponto);
    
}

// chama este método quando o botão cancelar da window ponto é clicado
static void on_buttonCancelarPoint_clicked() {

    gtk_widget_hide(windowPonto);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Inclusão de ponto cancelada!\n", -1);

}

// chama este método quando o botão salvar da window reta é clicado
static void on_buttonSalvarReta_clicked() {

    gtk_widget_hide(windowReta);

    double x1 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaX1));
    double y1 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaY1));
    double z1 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaZ1));

    double x2 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaX2));
    double y2 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaY2));
    double z2 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaZ2));

    string nome = gtk_entry_get_text(GTK_ENTRY(textEntryRetaName));

    desenharLinha(transformadaViewPortCoordenadaX(x1),
                  transformadaViewPortCoordenadaY(y1),
                  transformadaViewPortCoordenadaX(x2),
                  transformadaViewPortCoordenadaY(y2));

    std::ostringstream console;
    console << "A reta " << nome << "(" << x1 << ", " << y1 << ") -> (" << x2 << ", " << y2 << ") foi desenhada." << std::endl;
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);

    Reta *reta = new Reta(x1, y1, x2, y2, nome);
    objetosReta.push_back(reta);
}

// chama este método quando o botão cancelar da window reta é clicado
static void on_buttonCancelarReta_clicked() {

    gtk_widget_hide(windowReta);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Inclusão de reta cancelada!\n", -1);

}

// chama este método quando o botão add ponto da window poligono é clicado
static void on_buttonAddPontoAoPoligono_clicked() {
    double x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPoligonoX));
    double y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPoligonoY));
  
    Ponto *pontoAuxiliar = new Ponto(x, y);
    pontosAuxiliarPoligono.push_back(pontoAuxiliar);

    std::ostringstream console;
    console << "O ponto (" << x << ", " << y << ") foi adicionado a lista de pontos do seu polígono." << std::endl;
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);
    
}

// chama este método quando o botão salvar da window poligono é clicado
static void on_buttonSalvarPoligono_clicked() {

    gtk_widget_hide(windowPoligono);

    string nome = gtk_entry_get_text(GTK_ENTRY(textEntryPoligonoName));

    double x;
    double y;

    auto ponto = pontosAuxiliarPoligono.at(0);
      
    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, 5);
    cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); 
    cairo_move_to (cr, transformadaViewPortCoordenadaX(ponto->getValorX()),
                       transformadaViewPortCoordenadaY(ponto->getValorY()));

    for (std::vector<Ponto*>::iterator it = pontosAuxiliarPoligono.begin(); it != pontosAuxiliarPoligono.end(); ++it) {
        x = (*it)->getValorX();
        y = (*it)->getValorY();
        cairo_line_to (cr, transformadaViewPortCoordenadaX(x), transformadaViewPortCoordenadaY(y));
    }  

    cairo_line_to (cr, transformadaViewPortCoordenadaX(ponto->getValorX()),
                       transformadaViewPortCoordenadaY(ponto->getValorY()));

    cairo_stroke (cr);    
    gtk_widget_queue_draw (windowPrincipal);

    std::ostringstream console;
    console << "O poligono " << nome << " foi desenhado." << std::endl;
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);

    Poligono *poligono = new Poligono(pontosAuxiliarPoligono, nome);
    objetosPoligono.push_back(poligono);
    pontosAuxiliarPoligono.clear();
}

// chama este método quando o botão cancelar da window reta é clicado
static void on_buttonCancelarPoligono_clicked() {

    gtk_widget_hide(windowPoligono);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Inclusão de polígono cancelada!\n", -1);

}

/*Creates the surface*/
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
 
 if (surface) {
    cairo_surface_destroy (surface);
 }

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));
  clear_surface();
  
  return TRUE;

}

/* Redraw the screen from the surface */
static gboolean draw_cb (GtkWidget *widget, cairo_t   *cr,  gpointer   data) {
  
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);
  return FALSE;

}

int main(int argc, char *argv[]) {
   
    tela = Window(0, 0, 500, 500);
    
    GtkBuilder *builder;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "cg_ui.glade", NULL);

    windowPrincipal = GTK_WIDGET(gtk_builder_get_object(builder, "windowPrincipal"));
    windowPonto = GTK_WIDGET(gtk_builder_get_object(builder, "windowPonto"));
    windowReta = GTK_WIDGET(gtk_builder_get_object(builder, "windowReta"));
    windowPoligono = GTK_WIDGET(gtk_builder_get_object(builder, "windowPoligono"));
    drawingArea = GTK_WIDGET(gtk_builder_get_object(builder, "drawingArea"));
    
    textConsole = GTK_WIDGET(gtk_builder_get_object(builder, "textConsole"));
    objectsList = GTK_WIDGET(gtk_builder_get_object(builder, "objectsList"));
    scrolledConsole = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledConsole"));

    buttonPonto = GTK_WIDGET(gtk_builder_get_object(builder, "buttonPonto"));
    buttonPoligono = GTK_WIDGET(gtk_builder_get_object(builder, "buttonPoligono"));
    buttonReta = GTK_WIDGET(gtk_builder_get_object(builder, "buttonReta"));
    buttonCima = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCima"));
    buttonBaixo = GTK_WIDGET(gtk_builder_get_object(builder, "buttonBaixo"));
    buttonEsquerda = GTK_WIDGET(gtk_builder_get_object(builder, "buttonEsquerda"));
    buttonDireita = GTK_WIDGET(gtk_builder_get_object(builder, "buttonDireita"));
    buttonZoomIn = GTK_WIDGET(gtk_builder_get_object(builder, "buttonZoomIn"));
    buttonZoomOut = GTK_WIDGET(gtk_builder_get_object(builder, "buttonZoomOut"));
    buttonLimparTela = GTK_WIDGET(gtk_builder_get_object(builder, "buttonLimparTela"));

    buttonSalvarPoint = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSalvarPoint"));
    buttonCancelarPoint = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarPoint"));
    spinPontoX = GTK_WIDGET(gtk_builder_get_object(builder, "spinPontoX"));
    spinPontoY = GTK_WIDGET(gtk_builder_get_object(builder, "spinPontoY"));
    spinPontoZ = GTK_WIDGET(gtk_builder_get_object(builder, "spinPontoZ"));
    textEntryPointName = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryPointName"));

    buttonSalvarPoligono = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSalvarPoligono"));
    buttonCancelarPoligono = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarPoligono"));
    buttonAddPontoAoPoligono = GTK_WIDGET(gtk_builder_get_object(builder, "buttonAddPontoAoPoligono"));
    spinPoligonoX = GTK_WIDGET(gtk_builder_get_object(builder, "spinPoligonoX"));
    spinPoligonoY = GTK_WIDGET(gtk_builder_get_object(builder, "spinPoligonoY"));
    spinPoligonoZ = GTK_WIDGET(gtk_builder_get_object(builder, "spinPoligonoZ"));
    textEntryPoligonoName = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryPoligonoName"));

    buttonSalvarReta = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSalvarReta"));
    buttonCancelarReta = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarReta"));
    spinRetaX1 = GTK_WIDGET(gtk_builder_get_object(builder, "spinRetaX1"));
    spinRetaY1 = GTK_WIDGET(gtk_builder_get_object(builder, "spinRetaY1"));
    spinRetaZ1 = GTK_WIDGET(gtk_builder_get_object(builder, "spinRetaZ1"));
    spinRetaX2 = GTK_WIDGET(gtk_builder_get_object(builder, "spinRetaX2"));
    spinRetaY2 = GTK_WIDGET(gtk_builder_get_object(builder, "spinRetaY2"));
    spinRetaZ2 = GTK_WIDGET(gtk_builder_get_object(builder, "spinRetaZ2"));
    textEntryRetaName = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryRetaName"));

    g_signal_connect(buttonBaixo, "button-release-event", G_CALLBACK (on_buttonBaixo_clicked), NULL);
    g_signal_connect(buttonCima, "button-release-event", G_CALLBACK (on_buttonCima_clicked), NULL);
    g_signal_connect(buttonEsquerda, "button-release-event", G_CALLBACK (on_buttonEsquerda_clicked), NULL);
    g_signal_connect(buttonDireita, "button-release-event", G_CALLBACK (on_buttonDireita_clicked), NULL);
    g_signal_connect(buttonZoomIn, "button-release-event", G_CALLBACK (on_buttonZoomIn_clicked), NULL);
    g_signal_connect(buttonZoomOut, "button-release-event", G_CALLBACK (on_buttonZoomOut_clicked), NULL);
    g_signal_connect(buttonPonto, "button-release-event", G_CALLBACK (on_buttonPonto_clicked), NULL);
    g_signal_connect(buttonPoligono, "button-release-event", G_CALLBACK (on_buttonPoligono_clicked), NULL);
    g_signal_connect(buttonReta, "button-release-event", G_CALLBACK (on_buttonReta_clicked), NULL);
    g_signal_connect(buttonLimparTela, "button-release-event", G_CALLBACK (on_buttonLimparTela_clicked), NULL);

    g_signal_connect(buttonSalvarPoint, "button-release-event", G_CALLBACK (on_buttonSalvarPoint_clicked), NULL);
    g_signal_connect(buttonCancelarPoint, "button-release-event", G_CALLBACK (on_buttonCancelarPoint_clicked), NULL);

    g_signal_connect(buttonSalvarReta, "button-release-event", G_CALLBACK (on_buttonSalvarReta_clicked), NULL);
    g_signal_connect(buttonCancelarReta, "button-release-event", G_CALLBACK (on_buttonCancelarReta_clicked), NULL);

    g_signal_connect(buttonSalvarPoligono, "button-release-event", G_CALLBACK (on_buttonSalvarPoligono_clicked), NULL);
    g_signal_connect(buttonCancelarPoligono, "button-release-event", G_CALLBACK (on_buttonCancelarPoligono_clicked), NULL);
    g_signal_connect(buttonAddPontoAoPoligono, "button-release-event", G_CALLBACK (on_buttonAddPontoAoPoligono_clicked), NULL);
    
    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(drawingArea, "configure-event", G_CALLBACK(configure_event_cb), NULL);

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(windowPrincipal);                
    gtk_main();

    return 0;
}
