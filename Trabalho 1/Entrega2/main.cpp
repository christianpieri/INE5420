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
#include "Validator.hpp"

#define xViewPortMax 500
#define xViewPortMin 0
#define yViewPortMax 500
#define yViewPortMin 0

    GtkWidget *windowPrincipal;
    GtkWidget *windowPonto;
    GtkWidget *windowReta;
    GtkWidget *windowPoligono;
    GtkWidget *windowConfirmacaoExclusao;
    GtkWidget *windowAviso;
    GtkWidget *windowRotacionarObjeto;
    Window     tela;

    // Botões da caixa de controle
    GtkWidget *buttonZoomIn;
    GtkWidget *buttonZoomOut;
    GtkWidget *buttonCima;
    GtkWidget *buttonBaixo;
    GtkWidget *buttonEsquerda;
    GtkWidget *buttonDireita;
    GtkWidget *buttonLimparTela;
    GtkWidget *buttonRotateDireita;
    GtkWidget *buttonRotateEsquerda;
    GtkWidget *buttonDeletarObjeto;

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

    // Botões da window de confirmação de exclusão
    GtkWidget *buttonSimConfExclusao;
    GtkWidget *buttonCancelarConfExclusao;

    // Botões da window de erro
    GtkWidget *buttonOkWindowAviso;
    GtkWidget *mensagemAviso;
    GtkWidget *mensagemTituloAviso;
    
    // Área de desenho
    GtkWidget *drawingArea;
    
    // Console e objetos
    GtkWidget *textConsole;
    GtkScrolledWindow *scrolledConsole;
  
    // Tree view dos objetos
    GtkTreeView* objectTreeView;
    GtkListStore* objectListStore;
    GtkCellRenderer* objectsCellRenderer;
    GtkTreeSelection* objectSelected;
    
    // Lista de Objetos
    std::vector<Ponto*> objetosPonto;
    std::vector<Reta*> objetosReta;
    std::vector<Poligono*> objetosPoligono;
    std::vector<Ponto*> pontosAuxiliarPoligono;

    // Widgets da view de rotação
    GtkWidget *textEntryRotacaoPersonalizado;
    GtkToggleButton *radioButtonRotacaoOutro;
    GtkWidget *labelSentidoWindowRotacao;
    GtkWidget *buttonCancelarRotacao;
    GtkToggleButton *radioButtonPontoQualquer;
    GtkWidget *labelQualPontoRotacao;
    GtkWidget *textEntryQualX;
    GtkWidget *textEntryQualY;
    GtkWidget *buttonSalvarRotacao;

    // Surface
    static cairo_surface_t *surface = NULL;

    enum {
    COL_NAME = 0,
    COL_TYPE,
    NUM_COLS
};

// Transformada de ViewPort de X
static double transformadaViewPortCoordenadaX(double x) {
    double auxiliar = (x - tela.getValorXMinimo()) / (tela.getValorXMaximo() - tela.getValorXMinimo());

    return auxiliar * (xViewPortMax - xViewPortMin);
}

// Transformada de ViewPort de Y
static double transformadaViewPortCoordenadaY(double y) {   
    double auxiliar = (y - tela.getValorYMinimo()) / (tela.getValorYMaximo() - tela.getValorYMinimo());

    return (1 - auxiliar) * (yViewPortMax - yViewPortMin);
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
  
    gtk_widget_show(windowConfirmacaoExclusao);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão limpar tela pressionado!\n", -1);
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

    double xMaximo = tela.getValorXMaximo();
    double xMinimo = tela.getValorXMinimo();
    double yMaximo = tela.getValorYMaximo();
    double yMinimo = tela.getValorYMinimo();

    tela.setCoordenadasMaximo(xMaximo-5,yMaximo-5);
    tela.setCoordenadasMinimo(xMinimo+5,yMinimo+5);
    
    reDrawAll();
}

// chama este método quando o botão zoomOut é clicado
static void on_buttonZoomOut_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão zoom out pressionado!\n", -1);

    double xMaximo = tela.getValorXMaximo();
    double xMinimo = tela.getValorXMinimo();
    double yMaximo = tela.getValorYMaximo();
    double yMinimo = tela.getValorYMinimo();

    tela.setCoordenadasMaximo(xMaximo+5,yMaximo+5);
    tela.setCoordenadasMinimo(xMinimo-5,yMinimo-5);
    
    reDrawAll();
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

    string nome = gtk_entry_get_text(GTK_ENTRY(textEntryPointName));
    if(nome.empty()) {
            gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Objeto precisa ter um nome!");
            gtk_label_set_text(GTK_LABEL(mensagemAviso), "  Por favor, digite um nome para o seu novo objeto ponto.");
            gtk_widget_show(windowAviso);
    
    } else {

        double x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPontoX));
        double y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPontoY));
        double z = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPontoZ));


        bool nomeJaExistente = false;
        for (std::vector<Ponto*>::iterator it = objetosPonto.begin(); it != objetosPonto.end(); ++it)  {
            
            if(nome.compare((*it)->getNome()) == 0) {
                nomeJaExistente = true;
                break;
            }
        }
        
        if(nomeJaExistente) {
            gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Nome de objeto já existente!");
            gtk_label_set_text(GTK_LABEL(mensagemAviso), "Nome já utilizado em outro ponto! Por favor, troque-o.");
            gtk_widget_show(windowAviso);
        
        } else {

            gtk_widget_hide(windowPonto);

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

            GtkTreeIter iter;
            gtk_list_store_append(objectListStore, &iter);
            gtk_list_store_set(objectListStore, &iter, COL_NAME, gtk_entry_get_text(GTK_ENTRY(textEntryPointName)), COL_TYPE, "Ponto", -1);
        }
    }
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

    string nome = gtk_entry_get_text(GTK_ENTRY(textEntryRetaName));
        
    if(nome.empty()) {
        gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Objeto precisa ter um nome!");
        gtk_label_set_text(GTK_LABEL(mensagemAviso), " Por favor, digite um nome para o seu novo objeto reta.");
        gtk_widget_show(windowAviso);
    
    } else {

        bool nomeJaExistente = false;
        for (std::vector<Reta*>::iterator it = objetosReta.begin(); it != objetosReta.end(); ++it)  {
            if(nome.compare((*it)->getNome()) == 0) {
                nomeJaExistente = true;
                break;
            }
        }
        
        if(nomeJaExistente) {
            gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Nome de objeto já existente!");
            gtk_label_set_text(GTK_LABEL(mensagemAviso), "Nome já utilizado em outra reta! Por favor, troque-o.");
            gtk_widget_show(windowAviso);
        
        } else {

            gtk_widget_hide(windowReta);

            double x1 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaX1));
            double y1 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaY1));
            double z1 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaZ1));

            double x2 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaX2));
            double y2 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaY2));
            double z2 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinRetaZ2));

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

            GtkTreeIter iter;
            gtk_list_store_append(objectListStore, &iter);
            gtk_list_store_set(objectListStore, &iter, COL_NAME, gtk_entry_get_text(GTK_ENTRY(textEntryRetaName)), COL_TYPE, "Reta", -1);
        }
    }
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

    if(pontosAuxiliarPoligono.size() == 0) {
       
        gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Um ou mais dados estão faltando!");
        gtk_label_set_text(GTK_LABEL(mensagemAviso), "Você precisa adicionar ao menos 1 ponto ao seu novo objeto!  ");
        gtk_widget_show(windowAviso);

    } else {

        string nome = gtk_entry_get_text(GTK_ENTRY(textEntryPoligonoName));
        if(nome.empty()) {
            gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Objeto precisa ter um nome!");
            gtk_label_set_text(GTK_LABEL(mensagemAviso), "Por favor, digite um nome para o seu novo objeto polígono.");
            gtk_widget_show(windowAviso);
    
        } else {

            bool nomeJaExistente = false;
            for (std::vector<Poligono*>::iterator it = objetosPoligono.begin(); it != objetosPoligono.end(); ++it)  {
            
                if(nome.compare((*it)->getNome()) == 0) {
                    nomeJaExistente = true;
                    break;
                }
            }
        
            if(nomeJaExistente) {
                gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Nome de objeto já existente!");
                gtk_label_set_text(GTK_LABEL(mensagemAviso), "Nome já utilizado em outro polígono! Por favor, troque-o.");
                gtk_widget_show(windowAviso);
            
            } else {

                gtk_widget_hide(windowPoligono);

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

                GtkTreeIter iter;
                gtk_list_store_append(objectListStore, &iter);
                gtk_list_store_set(objectListStore, &iter, COL_NAME, gtk_entry_get_text(GTK_ENTRY(textEntryPoligonoName)), COL_TYPE, "Polígono", -1);
            }
        }
    }
}

// chama este método quando o botão cancelar da window reta é clicado
static void on_buttonCancelarPoligono_clicked() {

    gtk_widget_hide(windowPoligono);

    pontosAuxiliarPoligono.clear();

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Inclusão de polígono cancelada!\n", -1);

}

// chama este método quando o botão sim da window de confirmação de exclusão é clicado
static void on_buttonSimConfExclusao_clicked() {
    
    gtk_widget_hide(windowConfirmacaoExclusao);

    int quantidade = objetosPoligono.size() + objetosPonto.size() + objetosReta.size();

    std::ostringstream console;

    if(quantidade == 1) {
        console << "1 objeto foi excluído." << std::endl;
    } else {
        console << quantidade << " objetos foram excluídos." << std::endl;
    }
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);

    // TODO: LIMPAR TODA A LIST STORE

    objetosPoligono.clear();
    objetosReta.clear();
    objetosPonto.clear();
    
    clear_surface();
    gtk_widget_queue_draw (windowPrincipal);
}

static void on_buttonRotateDireita_clicked() {

    gtk_label_set_text(GTK_LABEL(labelSentidoWindowRotacao), "Rotacionando seu objeto para a direita:");
    gtk_widget_show(windowRotacionarObjeto);
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão rotação a direita pressionado!\n", -1);
}

static void on_buttonRotateEsquerda_clicked() {
    
    gtk_label_set_text(GTK_LABEL(labelSentidoWindowRotacao), "Rotacionando seu objeto para a esquerda:");
    gtk_widget_show(windowRotacionarObjeto);
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão rotação a esquerda pressionado!\n", -1);
}

static Ponto* retornarPonto() {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar* nomeDoObjeto;

    if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
        gtk_tree_model_get (model, &iter, COL_NAME, &nomeDoObjeto, -1);
        string nome = (std::string)nomeDoObjeto;
        std::vector<Ponto*>::iterator it;
        for (std::vector<Ponto*>::iterator it = objetosPonto.begin(); it != objetosPonto.end(); it++) {
            if(!nome.compare((*it)->getNome())) {
                return (*it);
            }
        }
    }
    return nullptr;
 }

static Reta* retornarReta() {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar* nomeDoObjeto;

    if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
        gtk_tree_model_get (model, &iter, COL_NAME, &nomeDoObjeto, -1);
        string nome = (std::string)nomeDoObjeto;
        std::vector<Reta*>::iterator it;
        for (std::vector<Reta*>::iterator it = objetosReta.begin(); it != objetosReta.end(); it++) {
            if(!nome.compare((*it)->getNome())) {
                return (*it);
            }
        }
    }
    return nullptr;
}

static Poligono* retornarPoligono() {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar* nomeDoObjeto;

    if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
        gtk_tree_model_get (model, &iter, COL_NAME, &nomeDoObjeto, -1);
        string nome = (std::string)nomeDoObjeto;
        std::vector<Poligono*>::iterator it;
        for (std::vector<Poligono*>::iterator it = objetosPoligono.begin(); it != objetosPoligono.end(); it++) {
            if(!nome.compare((*it)->getNome())) {
                return (*it);
            }
        }
    }
    return nullptr;
}

 static void deletarObjetoPonto() {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    auto ponto = retornarPonto();
    if(ponto == nullptr) {
        gtk_text_buffer_insert(buffer, &end, "Você precisa selecionar ao menos um objeto para deletá-lo!\n", -1);
    } else {
        for (int i = 0; i < objetosPonto.size(); i++) {
            if(objetosPonto.at(i)->getNome().compare(ponto->getNome()) == 0) {
                objetosPonto.erase(objetosPonto.begin() + i);

                std::ostringstream console;
                console << "O ponto " << ponto->getNome() << " foi deletado." << std::endl;
                gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);
                
                reDrawAll();

                GtkTreeIter iter;
                GtkTreeModel *model;
                gchar* nomeDoObjeto;
                if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
                    gtk_tree_model_get (model, &iter, 0, &nomeDoObjeto, -1);
                    gtk_list_store_remove(objectListStore, &iter);
                }
                
                break;
            }
        }
    }
 }

static void deletarObjetoReta() {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    auto reta = retornarReta();
    if(reta == nullptr) {
        gtk_text_buffer_insert(buffer, &end, "Você precisa selecionar ao menos um objeto para deletá-lo!\n", -1);
    } else {
        for (int i = 0; i < objetosReta.size(); i++) {
            if(objetosReta.at(i)->getNome().compare(reta->getNome()) == 0) {
                objetosReta.erase(objetosReta.begin() + i);

                std::ostringstream console;
                console << "A reta " << reta->getNome() << " foi deletada." << std::endl;
                gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);
                
                reDrawAll();

                GtkTreeIter iter;
                GtkTreeModel *model;
                gchar* nomeDoObjeto;
                if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
                    gtk_tree_model_get (model, &iter, 0, &nomeDoObjeto, -1);
                    gtk_list_store_remove(objectListStore, &iter);
                }
                
                break;
            }
        }
    }
}

static void deletarObjetoPoligono() {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    auto poligono = retornarPoligono();
    if(poligono == nullptr) {
        gtk_text_buffer_insert(buffer, &end, "Você precisa selecionar ao menos um objeto para deletá-lo!\n", -1);
    } else {
        for (int i = 0; i < objetosPoligono.size(); i++) {
            if(objetosPoligono.at(i)->getNome().compare(poligono->getNome()) == 0) {
                objetosPoligono.erase(objetosPoligono.begin() + i);

                std::ostringstream console;
                console << "O polígono " << poligono->getNome() << " foi deletado." << std::endl;
                gtk_text_buffer_insert(buffer, &end, console.str().c_str(), -1);
                
                reDrawAll();

                GtkTreeIter iter;
                GtkTreeModel *model;
                gchar* nomeDoObjeto;
                if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
                    gtk_tree_model_get (model, &iter, 0, &nomeDoObjeto, -1);
                    gtk_list_store_remove(objectListStore, &iter);
                }
                
                break;
            }
        }
    }
}

static void on_buttonDeletarObjeto_clicked() {
      
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão deletar objeto pressionado!\n", -1);
    

    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar* tipoDoObjeto;
    if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
        gtk_tree_model_get (model, &iter, COL_TYPE, &tipoDoObjeto, -1);
        string tipo = (std::string)tipoDoObjeto;
    
    if(tipo.compare("Ponto") == 0) {
        deletarObjetoPonto();
        
        } else if(tipo.compare("Reta") == 0) {
                deletarObjetoReta();
            
            } else if (tipo.compare("Polígono") == 0) {
                deletarObjetoPoligono();
        }

    } else {
        gtk_text_buffer_insert(buffer, &end, "Você precisa selecionar ao menos um objeto para deletá-lo!\n", -1);
    }
}

static void on_radioButtonRotacaoOutro_toggled() {
   
    if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioButtonRotacaoOutro))==TRUE) {
        gtk_widget_set_sensitive(textEntryRotacaoPersonalizado, true);
    } else {
       gtk_widget_set_sensitive(textEntryRotacaoPersonalizado, false);
    }
}

static void on_radioButtonPontoQualquer_toggled() {
   
    if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioButtonPontoQualquer))==TRUE) {
        gtk_widget_set_sensitive(labelQualPontoRotacao, true);
        gtk_widget_set_sensitive(textEntryQualX, true);
        gtk_widget_set_sensitive(textEntryQualY, true);
    } else {
        gtk_widget_set_sensitive(labelQualPontoRotacao, false);
        gtk_widget_set_sensitive(textEntryQualX, false);
        gtk_widget_set_sensitive(textEntryQualY, false);
    }
}

// chama este método quando o botão cancelar da window de rotação de objeto é clicado
static void on_buttonCancelarRotacao_clicked() {
    gtk_widget_hide(windowRotacionarObjeto);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Rotação de objeto cancelada!\n", -1);
}

// chama este método quando o botão salvar da window de rotação de objeto é clicado
static void on_buttonSalvarRotacao_clicked() {
    gtk_widget_hide(windowRotacionarObjeto);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Salvando rotação!\n", -1);
}

// chama este método quando o botão cancelar da window de confirmação de exclusão é clicado
static void on_buttonCancelarConfExclusao_clicked() {
    gtk_widget_hide(windowConfirmacaoExclusao);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Exclusão de objetos e limpeza de tela canceladas!\n", -1);
}

static void on_buttonOkWindowAviso_clicked() {
    gtk_widget_hide(windowAviso);
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

static void on_buttonFecharWindowPrincipal(GtkWidget *windowPrincipal, gpointer data) {
    gtk_main_quit();
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
    windowConfirmacaoExclusao = GTK_WIDGET(gtk_builder_get_object(builder, "windowConfirmacaoExclusao"));
    windowAviso = GTK_WIDGET(gtk_builder_get_object(builder, "windowAviso"));
    windowRotacionarObjeto = GTK_WIDGET(gtk_builder_get_object(builder, "windowRotacionarObjeto"));
    drawingArea = GTK_WIDGET(gtk_builder_get_object(builder, "drawingArea"));
    
    textConsole = GTK_WIDGET(gtk_builder_get_object(builder, "textConsole"));
    scrolledConsole = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledConsole"));

    objectTreeView = GTK_TREE_VIEW(gtk_builder_get_object(builder, "objectTreeView"));
    objectListStore = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
    objectsCellRenderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(objectTreeView, -1, "Nome", objectsCellRenderer, "text", COL_NAME, NULL);
    gtk_tree_view_insert_column_with_attributes(objectTreeView, -1, "Tipo", objectsCellRenderer, "text", COL_TYPE, NULL);
    gtk_tree_view_set_model(objectTreeView, GTK_TREE_MODEL(objectListStore));
    objectSelected = gtk_tree_view_get_selection(objectTreeView);
    gtk_tree_selection_set_mode(objectSelected, GTK_SELECTION_SINGLE);

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
    buttonRotateDireita = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRotateDireita"));
    buttonRotateEsquerda = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRotateEsquerda"));
    buttonDeletarObjeto = GTK_WIDGET(gtk_builder_get_object(builder, "buttonDeletarObjeto"));

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

    buttonSimConfExclusao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSimConfExclusao"));
    buttonCancelarConfExclusao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarConfExclusao"));

    buttonOkWindowAviso = GTK_WIDGET(gtk_builder_get_object(builder, "buttonOkWindowAviso"));
    mensagemTituloAviso = GTK_WIDGET(gtk_builder_get_object(builder, "mensagemTituloAviso"));
    mensagemAviso = GTK_WIDGET(gtk_builder_get_object(builder, "mensagemAviso"));

    textEntryRotacaoPersonalizado = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryRotacaoPersonalizado"));
    radioButtonRotacaoOutro = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "radioButtonRotacaoOutro"));
    radioButtonPontoQualquer = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "radioButtonPontoQualquer"));
    labelSentidoWindowRotacao = GTK_WIDGET(gtk_builder_get_object(builder, "labelSentidoWindowRotacao"));
    labelQualPontoRotacao = GTK_WIDGET(gtk_builder_get_object(builder, "labelQualPontoRotacao"));
    textEntryQualX = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryQualX"));
    textEntryQualY = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryQualY"));
    buttonCancelarRotacao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarRotacao"));
    buttonSalvarRotacao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSalvarRotacao"));

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
    g_signal_connect(buttonRotateDireita, "button-release-event", G_CALLBACK (on_buttonRotateDireita_clicked), NULL);
    g_signal_connect(buttonRotateEsquerda, "button-release-event", G_CALLBACK (on_buttonRotateEsquerda_clicked), NULL);
    g_signal_connect(buttonDeletarObjeto, "button-release-event", G_CALLBACK (on_buttonDeletarObjeto_clicked), NULL);

    g_signal_connect(buttonSalvarPoint, "button-release-event", G_CALLBACK (on_buttonSalvarPoint_clicked), NULL);
    g_signal_connect(buttonCancelarPoint, "button-release-event", G_CALLBACK (on_buttonCancelarPoint_clicked), NULL);

    g_signal_connect(buttonSalvarReta, "button-release-event", G_CALLBACK (on_buttonSalvarReta_clicked), NULL);
    g_signal_connect(buttonCancelarReta, "button-release-event", G_CALLBACK (on_buttonCancelarReta_clicked), NULL);

    g_signal_connect(buttonSalvarPoligono, "button-release-event", G_CALLBACK (on_buttonSalvarPoligono_clicked), NULL);
    g_signal_connect(buttonCancelarPoligono, "button-release-event", G_CALLBACK (on_buttonCancelarPoligono_clicked), NULL);
    g_signal_connect(buttonAddPontoAoPoligono, "button-release-event", G_CALLBACK (on_buttonAddPontoAoPoligono_clicked), NULL);

    g_signal_connect(buttonSimConfExclusao, "button-release-event", G_CALLBACK (on_buttonSimConfExclusao_clicked), NULL);
    g_signal_connect(buttonCancelarConfExclusao, "button-release-event", G_CALLBACK (on_buttonCancelarConfExclusao_clicked), NULL);
    
    g_signal_connect(buttonOkWindowAviso, "button-release-event", G_CALLBACK (on_buttonOkWindowAviso_clicked), NULL);

    g_signal_connect(radioButtonRotacaoOutro, "toggled", G_CALLBACK(on_radioButtonRotacaoOutro_toggled), NULL);
    g_signal_connect(radioButtonPontoQualquer, "toggled", G_CALLBACK(on_radioButtonPontoQualquer_toggled), NULL);
    g_signal_connect(buttonCancelarRotacao, "button-release-event", G_CALLBACK (on_buttonCancelarRotacao_clicked), NULL);
    g_signal_connect(buttonSalvarRotacao, "button-release-event", G_CALLBACK (on_buttonSalvarRotacao_clicked), NULL);

    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(drawingArea, "configure-event", G_CALLBACK(configure_event_cb), NULL);

    g_signal_connect(windowPrincipal, "destroy", G_CALLBACK(on_buttonFecharWindowPrincipal), NULL);

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(windowPrincipal);                
    gtk_main();

    return 0;
}
