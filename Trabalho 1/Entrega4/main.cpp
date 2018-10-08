using namespace std;
#include <gtk/gtk.h>
#include <iostream>
#include "Window.hpp"
#include "Ponto.hpp"
#include "Reta.hpp"
#include "Poligono.hpp"
#include "Curva.hpp"
#include "Clipping.cpp"
#include <sstream>
#include <vector>
#include <iterator>
#include "Validator.hpp"
#include <cmath>
#include "SaveLoadObj.cpp"
#include "RotacaoWindow.cpp"
#include <gdk/gdkkeysyms.h>

#define xViewPortMax 500.0
#define xViewPortMin 0.0
#define yViewPortMax 500.0
#define yViewPortMin 0.0
#define PI 3.14159265

    GtkWidget *windowPrincipal;
    GtkWidget *windowPonto;
    GtkWidget *windowReta;
    GtkWidget *windowPoligono;
    GtkWidget *windowConfirmacaoExclusao;
    GtkWidget *windowAviso;
    GtkWidget *windowRotacionarObjeto;
    GtkWidget *windowCurva;
    GtkWidget *windowEditarObjeto;
    GtkWidget *windowConfirmacaoCarregar;
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
    GtkWidget *buttonEditObjeto;
    GtkToggleButton *buttonOnOffClipping;
    GtkRadioButton *buttonRadioClip1;
    GtkRadioButton *buttonRadioClip2;
    GtkWidget *buttonSalvarObj;
    GtkWidget *buttonCarregarObj;

    // Botões dos objetos a serem desenhados
    GtkWidget *buttonPonto;
    GtkWidget *buttonReta;
    GtkWidget *buttonPoligono;
    GtkWidget *buttonCurva;

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

    // Botões da window curva
    GtkWidget *buttonSalvarCurva;
    GtkWidget *buttonCancelarCurva;
    GtkWidget *textEntryCurvaName;
    GtkWidget *spinCurvaX;
    GtkWidget *spinCurvaY;
    GtkWidget *spinCurvaZ;
    GtkWidget *buttonAddPontoACurva;
    GtkToggleButton *buttonRadioBezier;
    GtkWidget *labelWindowCurva;
    GtkWidget *labelQuantidadePontosCurva;
    GtkWidget *labelQuantidadePontosCurvaFaltantes;

    // Botões da window de confirmação de carregamento
    GtkWidget *buttonSimConfCarregar;
    GtkWidget *buttonCancelarConfCarregar;

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
    std::vector<Curva*> objetosCurva;
    std::vector<Ponto*> pontosAuxiliarPoligono;
    std::vector<Ponto*> pontosAuxiliarCurva;

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
    GtkWidget *radioButtonRotacao90;
    GtkWidget *radioButtonRotacao180;
    GtkWidget *radioButtonRotacao270;
    GtkWidget *radioButtonCentroMundo;
    GtkWidget *radioButtonCentroObjeto;
    std::string sentidoRotacao;

    // Botões da view de Editar Objeto
    GtkWidget *labelWindowEditar;
    GtkWidget *labelTransladarEscalonar;
    GtkWidget *textEntryEditarX;
    GtkWidget *textEntryEditarY;
    GtkWidget *textEntryEditarEscalonar;
    GtkWidget *buttonCancelarEdicao;
    GtkWidget *buttonSalvarEdicao;
    GtkToggleButton *radioButtonTransladar;
    GtkRadioButton *radioButtonEscalonar;

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

static void monstrarMensagemNoConsole(std::string mensagem) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, mensagem.c_str(), -1);
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

        if(gtk_toggle_button_get_active(buttonOnOffClipping) == TRUE) {
            if(!devoClipparPonto(x, y, &tela)) {
                desenharLinha(transformadaViewPortCoordenadaX(x),
                            transformadaViewPortCoordenadaY(y),
                            transformadaViewPortCoordenadaX(x),
                            transformadaViewPortCoordenadaY(y));
            }
        } else {
            desenharLinha(transformadaViewPortCoordenadaX(x),
                            transformadaViewPortCoordenadaY(y),
                            transformadaViewPortCoordenadaX(x),
                            transformadaViewPortCoordenadaY(y));
        }
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

// Redesenha curvas
static void redesenhaCurvas() {
    double x;
    double y;

    for (std::vector<Curva*>::iterator it = objetosCurva.begin(); it != objetosCurva.end(); ++it) {

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

        cairo_stroke (cr);    
    }
    gtk_widget_queue_draw (windowPrincipal);
}

// Redesenha as 4 retas da área de clipping
static void redesenhaAreaDeClipping() {
    if(gtk_toggle_button_get_active(buttonOnOffClipping) == TRUE) {
        cairo_t *cr;
        cr = cairo_create (surface);
        cairo_set_line_width (cr, 1);
        cairo_set_source_rgb(cr, 1, 0, 0);
        cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND);

        cairo_move_to (cr, 50, 50);
        cairo_line_to (cr, 50, 450);
        cairo_line_to (cr, 450, 450);
        cairo_line_to (cr, 450, 50);
        cairo_line_to (cr, 50, 50);
        
        cairo_stroke (cr);    
        gtk_widget_queue_draw (windowPrincipal);
    } else {
        // do nothing
    }
}

// Redesenha tudo
static void reDrawAll () {
    clear_surface();

    redesenhaAreaDeClipping();
    redesenhaPontos();
    redesenhaRetas();
    redesenhaPoligonos();
    redesenhaCurvas();
}

// chama este método quando o botão limpar tela é clicado
static void on_buttonLimparTela_clicked() {
  
    gtk_widget_show(windowConfirmacaoExclusao);
    monstrarMensagemNoConsole("Botão limpar tela pressionado!\n");
}

// chama este método quando o botão baixo é clicado
static void on_buttonBaixo_clicked() {
    
    monstrarMensagemNoConsole("Botão baixo pressionado! Movendo-se +10.\n");

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
    
    monstrarMensagemNoConsole("Botão cima pressionado! Movendo-se +10.\n");

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
    
    monstrarMensagemNoConsole("Botão esquerda pressionado! Movendo-se +10.\n");

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
    
   monstrarMensagemNoConsole("Botão direita pressionado! Movendo-se +10.\n");

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
    
    monstrarMensagemNoConsole("Botão zoom in pressionado!\n");

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
    
    monstrarMensagemNoConsole("Botão zoom out pressionado!\n");

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
    
    gtk_widget_show(windowReta);
    monstrarMensagemNoConsole("Botão reta pressionado!\n");
}

// chama este método quando o botão poligono é clicado
static void on_buttonPoligono_clicked() {
    
    gtk_widget_show(windowPoligono);
    monstrarMensagemNoConsole("Botão poligono pressionado!\n");
}

// chama este método quando o botão ponto é clicado
static void on_buttonPonto_clicked() {
    
    gtk_widget_show(windowPonto);
    monstrarMensagemNoConsole("Botão ponto pressionado!\n");
}


static void colocaObjetoNaListStore(std::string nome, std::string tipo) {
    GtkTreeIter iter;
    gtk_list_store_append(objectListStore, &iter);
    gtk_list_store_set(objectListStore, &iter, COL_NAME, nome.c_str(), COL_TYPE, tipo.c_str(), -1);
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
            monstrarMensagemNoConsole(console.str().c_str());

            Ponto *ponto = new Ponto(x, y, nome);
            objetosPonto.push_back(ponto);

            colocaObjetoNaListStore(nome, "Ponto");
        }
    }
}

// chama este método quando o botão cancelar da window ponto é clicado
static void on_buttonCancelarPoint_clicked() {

    gtk_widget_hide(windowPonto);
    monstrarMensagemNoConsole("Inclusão de ponto cancelada!\n");
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
            monstrarMensagemNoConsole(console.str().c_str());

            Reta *reta = new Reta(x1, y1, x2, y2, nome);
            objetosReta.push_back(reta);

            colocaObjetoNaListStore(nome, "Reta");
        }
    }
}

// chama este método quando o botão cancelar da window reta é clicado
static void on_buttonCancelarReta_clicked() {

    gtk_widget_hide(windowReta);
    monstrarMensagemNoConsole("Inclusão de reta cancelada!\n");

}

// chama este método quando o botão add ponto da window poligono é clicado
static void on_buttonAddPontoAoPoligono_clicked() {
    double x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPoligonoX));
    double y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinPoligonoY));
    
    Ponto *pontoAuxiliar = new Ponto(x, y);
    pontosAuxiliarPoligono.push_back(pontoAuxiliar);

    std::ostringstream console;
    console << "O ponto (" << x << ", " << y << ") foi adicionado a lista de pontos do seu polígono." << std::endl;
    monstrarMensagemNoConsole(console.str().c_str());
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
                monstrarMensagemNoConsole(console.str().c_str());

                Poligono *poligono = new Poligono(pontosAuxiliarPoligono, nome);
                objetosPoligono.push_back(poligono);
                pontosAuxiliarPoligono.clear();

                colocaObjetoNaListStore(nome, "Polígono");
            }
        }
    }
}

// chama este método quando o botão cancelar da window reta é clicado
static void on_buttonCancelarPoligono_clicked() {

    gtk_widget_hide(windowPoligono);
    pontosAuxiliarPoligono.clear();
    monstrarMensagemNoConsole("Inclusão de polígono cancelada!\n");
}

// chama este método quando o botão sim da window de confirmação de exclusão é clicado
static void on_buttonSimConfExclusao_clicked() {
    
    gtk_widget_hide(windowConfirmacaoExclusao);

    int quantidade = objetosCurva.size() +
                     objetosPoligono.size() + 
                     objetosPonto.size() + 
                     objetosReta.size();

    std::ostringstream console;

    if(quantidade == 1) {
        console << "1 objeto foi excluído." << std::endl;
    } else {
        console << quantidade << " objetos foram excluídos." << std::endl;
    }
    
    monstrarMensagemNoConsole(console.str().c_str());

    gtk_list_store_clear(objectListStore);
    objetosCurva.clear();
    objetosPoligono.clear();
    objetosReta.clear();
    objetosPonto.clear();
    
    clear_surface();
    reDrawAll();
    gtk_widget_queue_draw (windowPrincipal);
}

static std::string retornarTipoObjeto() {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar* tipoDoObjeto;
    if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
        gtk_tree_model_get (model, &iter, COL_TYPE, &tipoDoObjeto, -1);
        string tipo = (std::string)tipoDoObjeto;
        return tipo;
    } else {
        return "-1";
    }
}

static void rotacionaWindow() {
    objetosPonto = rotacionaTodosOsPontos(sentidoRotacao, objetosPonto, &tela);
    objetosReta = rotacionaTodasAsRetas(sentidoRotacao, objetosReta, &tela);
    objetosPoligono = rotacionaTodosOsPoligonos(sentidoRotacao, objetosPoligono, &tela);
    objetosCurva = rotacionaTodasAsCurvas(sentidoRotacao, objetosCurva, &tela);
    reDrawAll();
}

// chama este método quando o botão rotacionar a direita da window principal é clicado
static void on_buttonRotateDireita_clicked() {

    sentidoRotacao = "Direita";
    monstrarMensagemNoConsole("Botão rotação a direita pressionado!\n");

    std::string tipo = retornarTipoObjeto();
    if(tipo.compare("-1") == 0) {
        rotacionaWindow();
    } else {
        gtk_label_set_text(GTK_LABEL(labelSentidoWindowRotacao), "Rotacionando seu objeto para a direita:");
        gtk_widget_show(windowRotacionarObjeto);
    }
}

// chama este método quando o botão rotacionar a esquerda da window principal é clicado
static void on_buttonRotateEsquerda_clicked() {
    
    sentidoRotacao = "Esquerda";
    monstrarMensagemNoConsole("Botão rotação a esquerda pressionado!\n");

    std::string tipo = retornarTipoObjeto();
    if(tipo.compare("-1") == 0) {
        rotacionaWindow();
    } else {
        gtk_label_set_text(GTK_LABEL(labelSentidoWindowRotacao), "Rotacionando seu objeto para a esquerda:");
        gtk_widget_show(windowRotacionarObjeto);
    }
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

static Curva* retornarCurva() {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar* nomeDoObjeto;

    if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
        gtk_tree_model_get (model, &iter, COL_NAME, &nomeDoObjeto, -1);
        string nome = (std::string)nomeDoObjeto;
        std::vector<Curva*>::iterator it;
        for (std::vector<Curva*>::iterator it = objetosCurva.begin(); it != objetosCurva.end(); it++) {
            if(!nome.compare((*it)->getNome())) {
                return (*it);
            }
        }
    }
    return nullptr;
}

static void removerObjetoDaListStore() {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar* nomeDoObjeto;
    if (gtk_tree_selection_get_selected (objectSelected, &model, &iter)) {
        gtk_tree_model_get (model, &iter, 0, &nomeDoObjeto, -1);
        gtk_list_store_remove(objectListStore, &iter);
    }
}

static void deletarObjetoPonto() {
    
    auto ponto = retornarPonto();
    if(ponto == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para deletá-lo!\n");
    } else {
        for (int i = 0; i < objetosPonto.size(); i++) {
            if(objetosPonto.at(i)->getNome().compare(ponto->getNome()) == 0) {
                objetosPonto.erase(objetosPonto.begin() + i);

                std::ostringstream console;
                console << "O ponto " << ponto->getNome() << " foi deletado." << std::endl;
                monstrarMensagemNoConsole(console.str().c_str());
                
                reDrawAll();
                removerObjetoDaListStore();        
                break;
            }
        }
    }
 }

static void deletarObjetoReta() {
    
    auto reta = retornarReta();
    if(reta == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para deletá-lo!\n");
    } else {
        for (int i = 0; i < objetosReta.size(); i++) {
            if(objetosReta.at(i)->getNome().compare(reta->getNome()) == 0) {
                objetosReta.erase(objetosReta.begin() + i);

                std::ostringstream console;
                console << "A reta " << reta->getNome() << " foi deletada." << std::endl;
                monstrarMensagemNoConsole(console.str().c_str());
                
                reDrawAll();
                removerObjetoDaListStore();
                break;
            }
        }
    }
}

static void deletarObjetoPoligono() {
   
    auto poligono = retornarPoligono();
    if(poligono == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para deletá-lo!\n");
    } else {
        for (int i = 0; i < objetosPoligono.size(); i++) {
            if(objetosPoligono.at(i)->getNome().compare(poligono->getNome()) == 0) {
                objetosPoligono.erase(objetosPoligono.begin() + i);

                std::ostringstream console;
                console << "O polígono " << poligono->getNome() << " foi deletado." << std::endl;
                monstrarMensagemNoConsole(console.str().c_str());
                
                reDrawAll();
                removerObjetoDaListStore();                
                break;
            }
        }
    }
}

static void deletarObjetoCurva() {
   
    auto curva = retornarCurva();
    if(curva == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para deletá-lo!\n");
    } else {
        for (int i = 0; i < objetosCurva.size(); i++) {
            if(objetosCurva.at(i)->getNome().compare(curva->getNome()) == 0) {
                objetosCurva.erase(objetosCurva.begin() + i);

                std::ostringstream console;
                console << "A curva " << curva->getNome() << " foi deletada." << std::endl;
                monstrarMensagemNoConsole(console.str().c_str());
                
                reDrawAll();
                removerObjetoDaListStore();                
                break;
            }
        }
    }
}

// chama este método quando o botão deletar objeto é clicado
static void on_buttonDeletarObjeto_clicked() {
      
    monstrarMensagemNoConsole("Botão deletar objeto pressionado!\n");
    
    string tipo = retornarTipoObjeto();
    
    if(tipo.compare("-1") != 0) {
        if(tipo.compare("Ponto") == 0) {
            deletarObjetoPonto();
            
            } else if(tipo.compare("Reta") == 0) {
                    deletarObjetoReta();
                
                } else if(tipo.compare("Polígono") == 0) {
                        deletarObjetoPoligono();
                    } else if(tipo.compare("Curva") == 0) {
                        deletarObjetoCurva();
                    }
    } else {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para deletá-lo!\n");
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

// chama este método quando o botão salvar obj da window principal é clicado
static void on_buttonSalvarObj_clicked() {
        
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

    dialog = gtk_file_chooser_dialog_new("Save File",
                                        GTK_WINDOW(windowPrincipal),
                                        action,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_Save",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    gtk_file_chooser_set_current_name(chooser, "saida.obj");

    if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
        char *file;
        file = gtk_file_chooser_get_filename(chooser);
        if(file != NULL) {
            std::string filename(file);
            salvarObjetosPontoEmArquivo(objetosPonto, filename);
            salvarObjetosRetaEmArquivo(objetosReta, filename);
            salvarObjetosPoligonoEmArquivo(objetosPoligono, filename);
            salvarObjetosCurvaEmArquivo(objetosCurva, filename);
                
            monstrarMensagemNoConsole("Salvando no arquivo: " + filename + ".\n");
        }
        g_free (file);
    }
    gtk_widget_destroy (dialog);     
}

static void on_buttonCancelarConfCarregar_clicked() {
    monstrarMensagemNoConsole("Cancelado importação de arquivo .obj!\n");
    gtk_widget_hide(windowConfirmacaoCarregar);
}

static void on_buttonSimConfCarregar_clicked() {
    gtk_widget_hide(windowConfirmacaoCarregar);
    gtk_list_store_clear(objectListStore);
    objetosPoligono.clear();
    objetosReta.clear();
    objetosPonto.clear();
    objetosCurva.clear();
    clear_surface();

    std::vector<Poligono*> objetosVindosDoArquivo;
        
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                        GTK_WINDOW(windowPrincipal),
                                        action,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_Open",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *file;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        file = gtk_file_chooser_get_filename (chooser);
        if(file != NULL) {
            std::string filename(file);

            objetosVindosDoArquivo = lerObjetosDoArquivo(filename);
            for(int i = 0; i < objetosVindosDoArquivo.size(); i ++) {
                Poligono *p = objetosVindosDoArquivo.at(i);
                if(p->getListaDePontos().size() == 1) {
                    double x = p->getListaDePontos().at(0)->getValorX();
                    double y = p->getListaDePontos().at(0)->getValorY();
                    std::string nome = p->getNome();
                    Ponto *ponto = new Ponto(x, y, nome);
                    objetosPonto.push_back(ponto);
                    colocaObjetoNaListStore(nome, "Ponto");
                } else if(p->getListaDePontos().size() == 2) {
                    double x1 = p->getListaDePontos().at(0)->getValorX();
                    double y1 = p->getListaDePontos().at(0)->getValorY();
                    double x2 = p->getListaDePontos().at(1)->getValorX();
                    double y2 = p->getListaDePontos().at(1)->getValorY();
                    std::string nome = p->getNome();
                    Reta *reta = new Reta(x1, y1, x2, y2, nome);
                    objetosReta.push_back(reta);
                    colocaObjetoNaListStore(nome, "Reta");
                } else if(p->getListaDePontos().size() >= 20) {
                    std::string nome = p->getNome();
                    Curva *curva = new Curva(p->getListaDePontos(), nome);
                    objetosCurva.push_back(curva);
                    colocaObjetoNaListStore(nome, "Curva");
                } else {
                    std::string nome = p->getNome();
                    objetosPoligono.push_back(p);
                    colocaObjetoNaListStore(nome, "Polígono");
                }
            }
            monstrarMensagemNoConsole("Arquivo " + filename + " importado!\n");
            reDrawAll();
        }
        g_free(file);
    } else {
        monstrarMensagemNoConsole("Cancelado carregamento de arquivos.");
    }
    gtk_widget_destroy (dialog);
}


// chama este método quando o botão carregar obj da window principal é clicado
static void on_buttonCarregarObj_clicked() {
    monstrarMensagemNoConsole("Botão carregar objeto pressionado!\n");
    if(objetosCurva.size() + objetosPoligono.size() + objetosReta.size() + objetosPonto.size() > 0) {
        gtk_widget_show(windowConfirmacaoCarregar);    
    } else {
        on_buttonSimConfCarregar_clicked();
    }
}

static void refazLabelsWindowCurva() {
    int size = pontosAuxiliarCurva.size();
    std::ostringstream saidaLabelQuantidade;
    std::ostringstream saidaLabelFaltantes;
    saidaLabelQuantidade << "Você já adicionou " << size << " pontos a sua curva.";
    
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttonRadioBezier))==TRUE) {
        gtk_label_set_text(GTK_LABEL(labelQuantidadePontosCurva), saidaLabelQuantidade.str().c_str());
        if(size == 0 || (size % 4) != 0) {
            if(size < 4) {
                saidaLabelFaltantes << "Você precisa adicionar, pelo menos, mais " << fabs(4 - size) << " ponto(s)!";
            } else {
                saidaLabelFaltantes << "Você precisa adicionar, pelo menos, mais " << fabs(4 - size % 4) << " ponto(s)!";
            }
            gtk_widget_set_sensitive(buttonSalvarCurva, false);
            gtk_widget_set_tooltip_text(buttonSalvarCurva, saidaLabelFaltantes.str().c_str());

        } else {
            saidaLabelFaltantes << "Adição de pontos concluída. A curva já pode ser salva!";
            gtk_widget_set_sensitive(buttonSalvarCurva, true);
            gtk_widget_set_tooltip_text(buttonSalvarCurva, "");


        }
        gtk_label_set_text(GTK_LABEL(labelQuantidadePontosCurvaFaltantes), saidaLabelFaltantes.str().c_str());

    } else {
        gtk_label_set_text(GTK_LABEL(labelQuantidadePontosCurva), saidaLabelQuantidade.str().c_str());
        if(size < 4) {
            saidaLabelFaltantes << "Você precisa adicionar, pelo menos, mais " << 4 - size << " ponto(s)!";
            gtk_widget_set_sensitive(buttonSalvarCurva, false);
            gtk_widget_set_tooltip_text(buttonSalvarCurva, saidaLabelFaltantes.str().c_str());


        } else {
            saidaLabelFaltantes << "Adição de pontos concluída. A curva já pode ser salva!";
            gtk_widget_set_sensitive(buttonSalvarCurva, true);
            gtk_widget_set_tooltip_text(buttonSalvarCurva, "");

        }
        gtk_label_set_text(GTK_LABEL(labelQuantidadePontosCurvaFaltantes), saidaLabelFaltantes.str().c_str());

    }
}

// chama este método quando o botão desenhar curva da window principal é clicado
static void on_buttonCurva_clicked() {
    gtk_widget_show(windowCurva);
    monstrarMensagemNoConsole("Botão desenhar curva pressionado!\n");
    refazLabelsWindowCurva();
}

// chama este método quando o botão add ponto da window de curvas é clicado
static void on_buttonAddPontoACurva_clicked() {
    double x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinCurvaX));
    double y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinCurvaY));
    
    Ponto *pontoAuxiliar = new Ponto(x, y);
    pontosAuxiliarCurva.push_back(pontoAuxiliar);

    std::ostringstream console;
    console << "O ponto (" << x << ", " << y << ") foi adicionado a lista de pontos da sua curva." << std::endl;
    monstrarMensagemNoConsole(console.str().c_str());

    refazLabelsWindowCurva();
}

static void on_buttonRadioBezier_toggled() {
    
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttonRadioBezier))==TRUE) {
        gtk_label_set_text(GTK_LABEL(labelWindowCurva), "Escolha as coordenadas da sua nova curva de Bezier");
    } else {
        gtk_label_set_text(GTK_LABEL(labelWindowCurva), "Escolha as coordenadas da sua nova curva b-Spline");
    }
    
    refazLabelsWindowCurva();
}

// chama este método quando o botão salvar da window de curvas é clicado
static void on_buttonSalvarCurva_clicked() {
    
    string nome = gtk_entry_get_text(GTK_ENTRY(textEntryCurvaName));
    if(nome.empty()) {
            gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Objeto precisa ter um nome!");
            gtk_label_set_text(GTK_LABEL(mensagemAviso), "Por favor, digite um nome para o seu novo objeto curva.");
            gtk_widget_show(windowAviso);
    
    } else {

        bool nomeJaExistente = false;
        for (std::vector<Curva*>::iterator it = objetosCurva.begin(); it != objetosCurva.end(); ++it)  {
            
            if(nome.compare((*it)->getNome()) == 0) {
                nomeJaExistente = true;
                break;
            }
        }
        
        if(nomeJaExistente) {
            gtk_label_set_text(GTK_LABEL(mensagemTituloAviso), "Nome de objeto já existente!");
            gtk_label_set_text(GTK_LABEL(mensagemAviso), "Nome já utilizado em outra curva! Por favor, troque-o.");
            gtk_widget_show(windowAviso);
        } else {

            gtk_widget_hide(windowCurva);
            
            Ponto *p;
            double x;
            double y;

            std::vector<Ponto*> pontosConvertidosCurva;

            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttonRadioBezier))==TRUE) { 
                
                int i = 0;
                while(i < pontosAuxiliarCurva.size()) {
                    for(double t = 0; t < 1; t = t + 0.05) {
                        x = (pow((1 - t), 3) * pontosAuxiliarCurva.at(i + 0)->getValorX()) + 
                            (3 * t * pow((1 - t), 2) * pontosAuxiliarCurva.at(i + 1)->getValorX()) +
                            (3 * pow(t, 2) * (1 - t) * pontosAuxiliarCurva.at(i + 2)->getValorX()) +
                            (pow(t, 3) * pontosAuxiliarCurva.at(i + 3)->getValorX());

                        y = (pow((1 - t), 3) * pontosAuxiliarCurva.at(i + 0)->getValorY()) + 
                            (3 * t * pow((1 - t), 2) * pontosAuxiliarCurva.at(i + 1)->getValorY()) +
                            (3 * pow(t, 2) * (1 - t) * pontosAuxiliarCurva.at(i + 2)->getValorY()) +
                            (pow(t, 3) * pontosAuxiliarCurva.at(i + 3)->getValorY());

                        p = new Ponto(x, y);
                        pontosConvertidosCurva.push_back(p);
                    }

                    i = i + 4;
                }

            } else {
                int i = 0;
                double rangeX;
                double rangeY;
                double passo;

                while(i + 3 < pontosAuxiliarCurva.size()) {
                    rangeX = fabs(pontosAuxiliarCurva.at(i + 2)->getValorX() - pontosAuxiliarCurva.at(i + 1)->getValorX());
                    rangeY = fabs(pontosAuxiliarCurva.at(i + 2)->getValorY() - pontosAuxiliarCurva.at(i + 1)->getValorY());
                    if(rangeX > rangeY) {
                        passo = 1.0/rangeX;
                    } else {
                        passo = 1.0/rangeY;
                    }
                    
                    for(double t = 0; t <= 1; t = t + passo) {
                        x = (((-1*pow(t, 3) + 3*pow(t, 2) - 3*t + 1) * pontosAuxiliarCurva.at(i)->getValorX() +
                              ( 3*pow(t, 3) - 6*pow(t, 2) + 0*t + 4) * pontosAuxiliarCurva.at(i + 1)->getValorX() +
                              (-3*pow(t, 3) + 3*pow(t, 2) + 3*t + 1) * pontosAuxiliarCurva.at(i + 2)->getValorX() +
                              ( 1*pow(t, 3) + 0*pow(t, 2) + 0*t + 0) * pontosAuxiliarCurva.at(i + 3)->getValorX()) / 6);
                        y = (((-1*pow(t, 3) + 3*pow(t, 2) - 3*t + 1) * pontosAuxiliarCurva.at(i)->getValorY() +
                              ( 3*pow(t, 3) - 6*pow(t, 2) + 0*t + 4) * pontosAuxiliarCurva.at(i + 1)->getValorY() +
                              (-3*pow(t, 3) + 3*pow(t, 2) + 3*t + 1) * pontosAuxiliarCurva.at(i + 2)->getValorY() +
                              ( 1*pow(t, 3) + 0*pow(t, 2) + 0*t + 0) * pontosAuxiliarCurva.at(i + 3)->getValorY()) / 6);
                        
                        p = new Ponto(x, y);
                        pontosConvertidosCurva.push_back(p);
                    }
                    i++;
                }
            }

            Curva *curva = new Curva(pontosConvertidosCurva, nome);
            objetosCurva.push_back(curva);
            reDrawAll();
            pontosConvertidosCurva.clear();

            std::ostringstream console;
            console << "A curva " << nome << " foi desenhada." << std::endl;
            monstrarMensagemNoConsole(console.str().c_str());
            colocaObjetoNaListStore(nome, "Curva");
            pontosAuxiliarCurva.clear();
            refazLabelsWindowCurva();
        }    
    }
}

// chama este método quando o botão cancelar da window de curvas é clicado
static void on_buttonCancelarCurva_clicked() {
    gtk_widget_hide(windowCurva);
    pontosAuxiliarCurva.clear();
    monstrarMensagemNoConsole("Inclusão de curva cancelada!\n");
    refazLabelsWindowCurva();
}

// chama este método quando o botão cancelar da window de rotação de objeto é clicado
static void on_buttonCancelarRotacao_clicked() {
    gtk_widget_hide(windowRotacionarObjeto);
    monstrarMensagemNoConsole("Rotação de objeto cancelada!\n");
}

static double getValorAnguloRotacao() {
    double angulo;
        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonRotacao90))) == TRUE) {angulo = 90;}
        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonRotacao180))) == TRUE) {angulo = 180;}
        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonRotacao270))) == TRUE) {angulo = 270;}
        if(gtk_toggle_button_get_active(radioButtonRotacaoOutro) == TRUE) {
            angulo = atof(gtk_entry_get_text(GTK_ENTRY(textEntryRotacaoPersonalizado)));
        }

        if(sentidoRotacao.compare("Esquerda") == 0) {
            angulo = angulo * (-1);
        }

        return angulo = angulo * PI / 180;
}

static void rotacionarPonto() {
    auto ponto = retornarPonto();
    if(ponto == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {

        double angulo = getValorAnguloRotacao();

        double x;
        double y;

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroObjeto))) == TRUE) {
            
            x = ponto->getValorX();
            y = ponto->getValorY();
        }

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroMundo))) == TRUE) {
            x = 0;
            y = 0;
        }

        if(gtk_toggle_button_get_active(radioButtonPontoQualquer) == TRUE) {
            x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualX)));
            y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualY)));
        }
        
        double novoX = (ponto->getValorX() - x) * cos(angulo) + (ponto->getValorY() - y) * sin(angulo) + x;
        double novoY = (ponto->getValorX() - x) * -sin(angulo) + (ponto->getValorY() - y) * cos(angulo) + y;
        ponto->setValorX(novoX);
        ponto->setValorY(novoY);
               
        reDrawAll();
        std::ostringstream console;
        console << "O ponto " << ponto->getNome() << " foi rotacionado e agora é (" << 
                ponto->getValorX() << ", " << ponto->getValorY() << ")." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
    }
}

static void rotacionarReta() {
    auto reta = retornarReta();
    if(reta == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {

        double angulo = getValorAnguloRotacao();

        double x;
        double y;

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroObjeto))) == TRUE) {
            x = (reta->getValorXInicial() + reta->getValorXFinal())/2;
            y = (reta->getValorYInicial() + reta->getValorYFinal())/2;
        }

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroMundo))) == TRUE) {
            x = 0;
            y = 0;
        }

        if(gtk_toggle_button_get_active(radioButtonPontoQualquer) == TRUE) {
            x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualX)));
            y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualY)));
        }
        
        double novoXInicial = (reta->getValorXInicial() - x) * cos(angulo) + (reta->getValorYInicial() - y) * sin(angulo) + x;
        double novoYInicial = (reta->getValorXInicial() - x) * -sin(angulo) + (reta->getValorYInicial() - y) * cos(angulo) + y;
        double novoXFinal = (reta->getValorXFinal() - x) * cos(angulo) + (reta->getValorYFinal() - y) * sin(angulo) + x;
        double novoYFinal = (reta->getValorXFinal() - x) * -sin(angulo) + (reta->getValorYFinal() - y) * cos(angulo) + y;
        
        reta->setValorXInicial(novoXInicial);
        reta->setValorYInicial(novoYInicial);
        reta->setValorXFinal(novoXFinal);
        reta->setValorYFinal(novoYFinal);
        
        reDrawAll();
        std::ostringstream console;
        console << "A reta " << reta->getNome() << " foi rotacionada e agora é (" << 
            reta->getValorXInicial() << ", " << reta->getValorYInicial() << ") -> (" << 
            reta->getValorXFinal() << ", " << reta->getValorYFinal() << ")." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
    }
}

static void rotacionarPoligono() {
    auto poligono = retornarPoligono();
    if(poligono == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {

        double angulo = getValorAnguloRotacao();

        double x;
        double y;

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroObjeto))) == TRUE) {
            double somaX = 0;
            double somaY = 0;
        
            for(int i = 0; i < poligono->getListaDePontos().size(); i++) {
                auto ponto = poligono->getListaDePontos().at(i);
                somaX = somaX + ponto->getValorX();
                somaY = somaY + ponto->getValorY();
            }

            x = somaX/poligono->getListaDePontos().size();
            y = somaY/poligono->getListaDePontos().size();
        }

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroMundo))) == TRUE) {
            x = 0;
            y = 0;
        }

        if(gtk_toggle_button_get_active(radioButtonPontoQualquer) == TRUE) {
            x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualX)));
            y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualY)));
        }
        
        for(int i = 0; i < poligono->getListaDePontos().size(); i++) {
            auto ponto = poligono->getListaDePontos().at(i);
            double novoX = (ponto->getValorX() - x) * cos(angulo) + (ponto->getValorY() - y) * sin(angulo) + x;
            double novoY = (ponto->getValorX() - x) * -sin(angulo) + (ponto->getValorY() - y) * cos(angulo) + y;
            ponto->setValorX(novoX);
            ponto->setValorY(novoY);
        }
       
        reDrawAll();
        std::ostringstream console;
        console << "O polígono " << poligono->getNome() << " foi rotacionado." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
    }
}

static void rotacionarCurva() {
    auto curva = retornarCurva();
    if(curva == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {

        double angulo = getValorAnguloRotacao();

        double x;
        double y;

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroObjeto))) == TRUE) {
            double somaX = 0;
            double somaY = 0;
        
            for(int i = 0; i < curva->getListaDePontos().size(); i++) {
                auto ponto = curva->getListaDePontos().at(i);
                somaX = somaX + ponto->getValorX();
                somaY = somaY + ponto->getValorY();
            }

            x = somaX/curva->getListaDePontos().size();
            y = somaY/curva->getListaDePontos().size();
        }

        if(gtk_toggle_button_get_active((GTK_TOGGLE_BUTTON(radioButtonCentroMundo))) == TRUE) {
            x = 0;
            y = 0;
        }

        if(gtk_toggle_button_get_active(radioButtonPontoQualquer) == TRUE) {
            x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualX)));
            y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryQualY)));
        }
        
        for(int i = 0; i < curva->getListaDePontos().size(); i++) {
            auto ponto = curva->getListaDePontos().at(i);
            double novoX = (ponto->getValorX() - x) * cos(angulo) + (ponto->getValorY() - y) * sin(angulo) + x;
            double novoY = (ponto->getValorX() - x) * -sin(angulo) + (ponto->getValorY() - y) * cos(angulo) + y;
            ponto->setValorX(novoX);
            ponto->setValorY(novoY);
        }
       
        reDrawAll();
        std::ostringstream console;
        console << "O polígono " << curva->getNome() << " foi rotacionado." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
    }
}

// chama este método quando o botão salvar da window de rotação de objeto é clicado
static void on_buttonSalvarRotacao_clicked() {
    gtk_widget_hide(windowRotacionarObjeto);

    std::string tipo = retornarTipoObjeto();

    if(tipo.compare("-1") != 0) {
        if(tipo.compare("Ponto") == 0) {
                rotacionarPonto();
            } else if(tipo.compare("Reta") == 0) {
                    rotacionarReta();
                    } else if(tipo.compare("Polígono") == 0) {
                            rotacionarPoligono();
                        } else if(tipo.compare("Curva") == 0) {
                                rotacionarCurva();
                        }
    } else {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    }
}

// chama este método quando o botão cancelar da window de confirmação de exclusão é clicado
static void on_buttonCancelarConfExclusao_clicked() {
    gtk_widget_hide(windowConfirmacaoExclusao);
    monstrarMensagemNoConsole("Exclusão de objetos e limpeza de tela canceladas!\n");
}

// on - off do clipping
static void on_buttonOnOffClipping_toggled() {
    if(gtk_toggle_button_get_active(buttonOnOffClipping) == FALSE) {
        gtk_button_set_label(GTK_BUTTON(buttonOnOffClipping), "Off");
        gtk_widget_set_tooltip_text(GTK_WIDGET(GTK_BUTTON(buttonOnOffClipping)), "Clicar ativará o clipping de objetos");
        gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(buttonRadioClip1)), false);
        gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(buttonRadioClip2)), false);
        monstrarMensagemNoConsole("Clipping de objetos desativado!\n");
    } else {
        gtk_button_set_label(GTK_BUTTON(buttonOnOffClipping), "On");
        gtk_widget_set_tooltip_text(GTK_WIDGET(GTK_BUTTON(buttonOnOffClipping)), "Clicar desativará o clipping de objetos");
        gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(buttonRadioClip1)), true);
        gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(buttonRadioClip2)), true);
        monstrarMensagemNoConsole("Clipping de objetos ativado!\n");
    }

    reDrawAll();
}

// chama quando o botão ok da window aviso é clicado
static void on_buttonOkWindowAviso_clicked() {
    gtk_widget_hide(windowAviso);
}

// chama para fazer a troca de transladar com escalonar
static void on_radioButtonTransladar_toggled() {
   
    if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioButtonTransladar))==TRUE) {
        gtk_widget_set_sensitive(textEntryEditarX, true);
        gtk_widget_set_sensitive(textEntryEditarY, true);
        gtk_widget_set_sensitive(textEntryEditarEscalonar, false);
        gtk_label_set_text(GTK_LABEL(labelTransladarEscalonar), "Você gostaria de transladar o seu objeto em quantos x e y?");
    } else {
        gtk_widget_set_sensitive(textEntryEditarX, false);
        gtk_widget_set_sensitive(textEntryEditarY, false);
        gtk_widget_set_sensitive(textEntryEditarEscalonar, true);
        gtk_label_set_text(GTK_LABEL(labelTransladarEscalonar), "Você gostaria de escalonar o seu objeto em quantos %?");
    }
}

static void editarObjetoPonto() {
    gtk_toggle_button_set_active(radioButtonTransladar, true);
    gtk_label_set_text(GTK_LABEL(labelWindowEditar), "Editando seu objeto ponto:");
    gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), false);
    gtk_label_set_text(GTK_LABEL(labelTransladarEscalonar), "Você gostaria de transladar  o seu objeto em quantos x e y?");
    gtk_widget_set_sensitive(textEntryEditarEscalonar, false);
    gtk_widget_set_tooltip_text(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), "Um objeto do tipo ponto não pode ser escalonado");
    gtk_widget_show(windowEditarObjeto);
}

static void editarObjetoReta() {
    gtk_toggle_button_set_active(radioButtonTransladar, true);
    gtk_label_set_text(GTK_LABEL(labelWindowEditar), "Editando seu objeto reta:");
    gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), true);
    gtk_label_set_text(GTK_LABEL(labelTransladarEscalonar), "Você gostaria de transladar  o seu objeto em quantos x e y?");
    gtk_widget_set_sensitive(textEntryEditarEscalonar, true);
    gtk_widget_set_tooltip_text(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), "");
    on_radioButtonTransladar_toggled();
    gtk_widget_show(windowEditarObjeto);
}

static void editarObjetoPoligono() {
    gtk_toggle_button_set_active(radioButtonTransladar, true);
    gtk_label_set_text(GTK_LABEL(labelWindowEditar), "Editando seu objeto polígono:");
    gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), true);
    gtk_label_set_text(GTK_LABEL(labelTransladarEscalonar), "Você gostaria de transladar o seu objeto em quantos x e y?");
    gtk_widget_set_sensitive(textEntryEditarEscalonar, true);
    gtk_widget_set_tooltip_text(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), "");
    on_radioButtonTransladar_toggled();
    gtk_widget_show(windowEditarObjeto);
}

static void editarObjetoCurva() {
    gtk_toggle_button_set_active(radioButtonTransladar, true);
    gtk_label_set_text(GTK_LABEL(labelWindowEditar), "Editando seu objeto curva:");
    gtk_widget_set_sensitive(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), true);
    gtk_label_set_text(GTK_LABEL(labelTransladarEscalonar), "Você gostaria de transladar o seu objeto em quantos x e y?");
    gtk_widget_set_sensitive(textEntryEditarEscalonar, true);
    gtk_widget_set_tooltip_text(GTK_WIDGET(GTK_BUTTON(GTK_RADIO_BUTTON(radioButtonEscalonar))), "");
    on_radioButtonTransladar_toggled();
    gtk_widget_show(windowEditarObjeto);
}

// chama quando botão editar objeto é clicado
static void on_buttonEditObjeto_clicked() {

    monstrarMensagemNoConsole("Botão editar objeto pressionado!\n");

    std::string tipo = retornarTipoObjeto();

    if(tipo.compare("Ponto") == 0) {
        editarObjetoPonto();
        
        } else if(tipo.compare("Reta") == 0) {
                editarObjetoReta();
            
            } else if(tipo.compare("Polígono") == 0) {
                editarObjetoPoligono();

                } else if(tipo.compare("Curva") == 0) {
                    editarObjetoCurva();
                    
                    } else {
                        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
                    }
}

// chama quando botão cancelar da edicao é clicado
static void on_buttonCancelarEdicao_clicked() {
    gtk_widget_hide(windowEditarObjeto);
    monstrarMensagemNoConsole("Edição de objeto cancelada!\n");
}

static void transladarPonto() {
       
    auto ponto = retornarPonto();
    if(ponto == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {
        
        double x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarX)));
        double y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarY)));
        
        ponto->setValorX(ponto->getValorX() + x);
        ponto->setValorY(ponto->getValorY() + y);
            
        std::ostringstream console;
        console << "O ponto " << ponto->getNome() << " foi transladado para o local (" << 
                    ponto->getValorX() << ", " << ponto->getValorY() << ")." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
        reDrawAll();
    }        
}

static void transladarReta() {
    auto reta = retornarReta();
    if(reta == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {
        
        double x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarX)));
        double y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarY)));
        
        reta->setValorXInicial(reta->getValorXInicial() + x);
        reta->setValorYInicial(reta->getValorYInicial() + y);
        reta->setValorXFinal(reta->getValorXFinal() + x);
        reta->setValorYFinal(reta->getValorYFinal() + y);
            
        std::ostringstream console;
        console << "A reta " << reta->getNome() << " foi transladada para o local (" << 
            reta->getValorXInicial() << ", " << reta->getValorYInicial() << ") -> (" << 
            reta->getValorXFinal() << ", " << reta->getValorYFinal() << ")." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
        reDrawAll();
    }
}

static void transladarPoligono() {
    auto poligono = retornarPoligono();
    if(poligono == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {
        
        double x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarX)));
        double y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarY)));
       
        for(int i = 0; i < poligono->getListaDePontos().size(); i++) {
            auto ponto = poligono->getListaDePontos().at(i);
            ponto->setValorX(ponto->getValorX() + x);
            ponto->setValorY(ponto->getValorY() + y);
        }
            
        std::ostringstream console;
        console << "O polígono " << poligono->getNome() << " foi transladado em (" << 
                x << ", " << y << ") a mais." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
        reDrawAll();
    }
}

static void transladarCurva() {
    auto curva = retornarCurva();
    if(curva == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {
        
        double x = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarX)));
        double y = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarY)));
       
        for(int i = 0; i < curva->getListaDePontos().size(); i++) {
            auto ponto = curva->getListaDePontos().at(i);
            ponto->setValorX(ponto->getValorX() + x);
            ponto->setValorY(ponto->getValorY() + y);
        }
            
        std::ostringstream console;
        console << "A curva " << curva->getNome() << " foi transladada em (" << 
                x << ", " << y << ") a mais." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
        reDrawAll();
    }
}

static void escalonarReta() {
    auto reta = retornarReta();
    if(reta == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {

        double recebido = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarEscalonar)));

        double pontoMedioX = (reta->getValorXInicial() + reta->getValorXFinal())/2;
        double pontoMedioY = (reta->getValorYInicial() + reta->getValorYFinal())/2;
        
        double novoXInicial = (reta->getValorXInicial() - pontoMedioX) * recebido + pontoMedioX;
        double novoYInicial = (reta->getValorYInicial() - pontoMedioY) * recebido + pontoMedioY;
        double novoXFinal = (reta->getValorXFinal() - pontoMedioX) * recebido + pontoMedioX;
        double novoYFinal = (reta->getValorYFinal() - pontoMedioY) * recebido + pontoMedioY;
        
        reta->setValorXInicial(novoXInicial);
        reta->setValorYInicial(novoYInicial);
        reta->setValorXFinal(novoXFinal);
        reta->setValorYFinal(novoYFinal);

        reDrawAll();
        std::ostringstream console;
        console << "A reta " << reta->getNome() << " foi escalonada e agora é (" << 
            reta->getValorXInicial() << ", " << reta->getValorYInicial() << ") -> (" << 
            reta->getValorXFinal() << ", " << reta->getValorYFinal() << ")." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
    }
}

static void escalonarPoligono() {
    auto poligono = retornarPoligono();
    if(poligono == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {

        double recebido = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarEscalonar)));

        double somaX = 0;
        double somaY = 0;
        
        for(int i = 0; i < poligono->getListaDePontos().size(); i++) {
            auto ponto = poligono->getListaDePontos().at(i);
            somaX = somaX + ponto->getValorX();
            somaY = somaY + ponto->getValorY();
        }

        double pontoMedioX = somaX/poligono->getListaDePontos().size();
        double pontoMedioY = somaY/poligono->getListaDePontos().size();

        for(int i = 0; i < poligono->getListaDePontos().size(); i++) {
            auto ponto = poligono->getListaDePontos().at(i);
            ponto->setValorX((ponto->getValorX() - pontoMedioX) * recebido + pontoMedioX);
            ponto->setValorY((ponto->getValorY() - pontoMedioY) * recebido + pontoMedioY);
        }


        reDrawAll();
        std::ostringstream console;
        console << "O polígono " << poligono->getNome() << " foi escalonado." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
    }
}

static void escalonarCurva() {
    auto curva = retornarCurva();
    if(curva == nullptr) {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    } else {

        double recebido = atof(gtk_entry_get_text(GTK_ENTRY(textEntryEditarEscalonar)));

        double somaX = 0;
        double somaY = 0;
        
        for(int i = 0; i < curva->getListaDePontos().size(); i++) {
            auto ponto = curva->getListaDePontos().at(i);
            somaX = somaX + ponto->getValorX();
            somaY = somaY + ponto->getValorY();
        }

        double pontoMedioX = somaX/curva->getListaDePontos().size();
        double pontoMedioY = somaY/curva->getListaDePontos().size();

        for(int i = 0; i < curva->getListaDePontos().size(); i++) {
            auto ponto = curva->getListaDePontos().at(i);
            ponto->setValorX((ponto->getValorX() - pontoMedioX) * recebido + pontoMedioX);
            ponto->setValorY((ponto->getValorY() - pontoMedioY) * recebido + pontoMedioY);
        }

        reDrawAll();
        std::ostringstream console;
        console << "A curva " << curva->getNome() << " foi escalonada." << std::endl;
        monstrarMensagemNoConsole(console.str().c_str());
    }
}

// chama quando botão salvar da edição é clicado
static void on_buttonSalvarEdicao_clicked() {
    gtk_widget_hide(windowEditarObjeto);

    std::string tipo = retornarTipoObjeto();

    if(tipo.compare("-1") != 0) {
        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioButtonTransladar))==TRUE) {
            if(tipo.compare("Ponto") == 0) {
                transladarPonto();
            } else if(tipo.compare("Reta") == 0) {
                    transladarReta();
                    } else if (tipo.compare("Polígono") == 0) {
                            transladarPoligono();
                        } else if(tipo.compare("Curva") == 0) {
                                transladarCurva();
                        }
            } else {
                if(tipo.compare("Ponto") == 0) {
                    monstrarMensagemNoConsole("Você é o Magaiver? Um objeto ponto não pode ser escalonado");
                    } else if(tipo.compare("Reta") == 0) {
                            escalonarReta();
                        
                        } else if (tipo.compare("Polígono") == 0) {
                            escalonarPoligono();   
                            } else if(tipo.compare("Curva") == 0) {
                                escalonarCurva();
                            }      
            }
    } else {
        monstrarMensagemNoConsole("Você precisa selecionar ao menos um objeto para editá-lo!\n");
    }
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

static void on_qualquerTeclaTeclado_Pressed(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if(event->keyval == GDK_KEY_F1) {
        system("xdg-open https://christianpieri.github.io/docCg/");
    } 
    
    if(event->keyval == GDK_KEY_Up) {
         on_buttonCima_clicked();
    }

    if(event->keyval == GDK_KEY_Down) {
         on_buttonBaixo_clicked();
    }

    if(event->keyval == GDK_KEY_Left) {
         on_buttonEsquerda_clicked();
    }

    if(event->keyval == GDK_KEY_Right) {
         on_buttonDireita_clicked();
    }

    if(event->keyval == GDK_KEY_Delete) {
         on_buttonDeletarObjeto_clicked();
    }

    if(event->keyval == GDK_KEY_minus || event->keyval == GDK_KEY_KP_Subtract) {
         on_buttonZoomOut_clicked();
    }

    if(event->keyval == GDK_KEY_plus || event->keyval == GDK_KEY_KP_Add) {
         on_buttonZoomIn_clicked();
    }

    if(event->keyval == GDK_KEY_s && GDK_CONTROL_MASK) {
        on_buttonSalvarObj_clicked();
    }

    if(event->keyval == GDK_KEY_o && GDK_CONTROL_MASK) {
        on_buttonCarregarObj_clicked();
    }
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
    windowCurva = GTK_WIDGET(gtk_builder_get_object(builder, "windowCurva"));
    windowEditarObjeto = GTK_WIDGET(gtk_builder_get_object(builder, "windowEditarObjeto"));
    windowConfirmacaoCarregar = GTK_WIDGET(gtk_builder_get_object(builder, "windowConfirmacaoCarregar"));
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
    buttonCurva = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCurva"));
    buttonOnOffClipping = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "buttonOnOffClipping"));
    buttonRadioClip1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "buttonRadioClip1"));
    buttonRadioClip2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "buttonRadioClip2"));
    buttonSalvarObj = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSalvarObj"));
    buttonCarregarObj = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCarregarObj"));
    buttonEditObjeto = GTK_WIDGET(gtk_builder_get_object(builder, "buttonEditObjeto"));
    
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

    buttonSalvarCurva = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSalvarCurva"));
    buttonCancelarCurva = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarCurva"));
    textEntryCurvaName = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryCurvaName"));
    spinCurvaX = GTK_WIDGET(gtk_builder_get_object(builder, "spinCurvaX"));
    spinCurvaY = GTK_WIDGET(gtk_builder_get_object(builder, "spinCurvaY"));
    spinCurvaZ = GTK_WIDGET(gtk_builder_get_object(builder, "spinCurvaZ"));
    buttonAddPontoACurva = GTK_WIDGET(gtk_builder_get_object(builder, "buttonAddPontoACurva"));
    buttonRadioBezier = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "buttonRadioBezier"));
    labelWindowCurva = GTK_WIDGET(gtk_builder_get_object(builder, "labelWindowCurva"));
    labelQuantidadePontosCurva = GTK_WIDGET(gtk_builder_get_object(builder, "labelQuantidadePontosCurva"));
    labelQuantidadePontosCurvaFaltantes = GTK_WIDGET(gtk_builder_get_object(builder, "labelQuantidadePontosCurvaFaltantes"));
    
    buttonSimConfExclusao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSimConfExclusao"));
    buttonCancelarConfExclusao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarConfExclusao"));

    buttonSimConfCarregar = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSimConfCarregar"));
    buttonCancelarConfCarregar = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarConfCarregar"));

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
    radioButtonRotacao90 = GTK_WIDGET(gtk_builder_get_object(builder, "radioButtonRotacao90"));
    radioButtonRotacao180 = GTK_WIDGET(gtk_builder_get_object(builder, "radioButtonRotacao180"));
    radioButtonRotacao270 = GTK_WIDGET(gtk_builder_get_object(builder, "radioButtonRotacao270"));
    radioButtonCentroMundo = GTK_WIDGET(gtk_builder_get_object(builder, "radioButtonCentroMundo"));
    radioButtonCentroObjeto = GTK_WIDGET(gtk_builder_get_object(builder, "radioButtonCentroObjeto"));

    labelWindowEditar = GTK_WIDGET(gtk_builder_get_object(builder, "labelWindowEditar"));
    labelTransladarEscalonar = GTK_WIDGET(gtk_builder_get_object(builder, "labelTransladarEscalonar"));
    textEntryEditarX = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryEditarX"));
    textEntryEditarY = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryEditarY"));
    textEntryEditarEscalonar = GTK_WIDGET(gtk_builder_get_object(builder, "textEntryEditarEscalonar"));
    buttonSalvarEdicao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSalvarEdicao"));
    buttonCancelarEdicao = GTK_WIDGET(gtk_builder_get_object(builder, "buttonCancelarEdicao"));
    radioButtonTransladar = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "radioButtonTransladar"));
    radioButtonEscalonar = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radioButtonEscalonar"));

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
    g_signal_connect(buttonCurva, "button-release-event", G_CALLBACK (on_buttonCurva_clicked), NULL);
    g_signal_connect(buttonOnOffClipping, "toggled", G_CALLBACK(on_buttonOnOffClipping_toggled), NULL);
    g_signal_connect(buttonSalvarObj, "button-release-event", G_CALLBACK(on_buttonSalvarObj_clicked), NULL);
    g_signal_connect(buttonCarregarObj, "button-release-event", G_CALLBACK(on_buttonCarregarObj_clicked), NULL);
    g_signal_connect(buttonEditObjeto, "button-release-event", G_CALLBACK(on_buttonEditObjeto_clicked), NULL);
    
    g_signal_connect(buttonSalvarPoint, "button-release-event", G_CALLBACK (on_buttonSalvarPoint_clicked), NULL);
    g_signal_connect(buttonCancelarPoint, "button-release-event", G_CALLBACK (on_buttonCancelarPoint_clicked), NULL);

    g_signal_connect(buttonSalvarReta, "button-release-event", G_CALLBACK (on_buttonSalvarReta_clicked), NULL);
    g_signal_connect(buttonCancelarReta, "button-release-event", G_CALLBACK (on_buttonCancelarReta_clicked), NULL);

    g_signal_connect(buttonSalvarPoligono, "button-release-event", G_CALLBACK (on_buttonSalvarPoligono_clicked), NULL);
    g_signal_connect(buttonCancelarPoligono, "button-release-event", G_CALLBACK (on_buttonCancelarPoligono_clicked), NULL);
    g_signal_connect(buttonAddPontoAoPoligono, "button-release-event", G_CALLBACK (on_buttonAddPontoAoPoligono_clicked), NULL);

    g_signal_connect(buttonSalvarCurva, "button-release-event", G_CALLBACK (on_buttonSalvarCurva_clicked), NULL);
    g_signal_connect(buttonCancelarCurva, "button-release-event", G_CALLBACK (on_buttonCancelarCurva_clicked), NULL);
    g_signal_connect(buttonAddPontoACurva, "button-release-event", G_CALLBACK (on_buttonAddPontoACurva_clicked), NULL);
    g_signal_connect(buttonRadioBezier, "toggled", G_CALLBACK(on_buttonRadioBezier_toggled), NULL);

    g_signal_connect(buttonSimConfExclusao, "button-release-event", G_CALLBACK (on_buttonSimConfExclusao_clicked), NULL);
    g_signal_connect(buttonCancelarConfExclusao, "button-release-event", G_CALLBACK (on_buttonCancelarConfExclusao_clicked), NULL);

    g_signal_connect(buttonSimConfCarregar, "button-release-event", G_CALLBACK (on_buttonSimConfCarregar_clicked), NULL);
    g_signal_connect(buttonCancelarConfCarregar, "button-release-event", G_CALLBACK (on_buttonCancelarConfCarregar_clicked), NULL);
    
    g_signal_connect(buttonOkWindowAviso, "button-release-event", G_CALLBACK (on_buttonOkWindowAviso_clicked), NULL);

    g_signal_connect(radioButtonRotacaoOutro, "toggled", G_CALLBACK(on_radioButtonRotacaoOutro_toggled), NULL);
    g_signal_connect(radioButtonPontoQualquer, "toggled", G_CALLBACK(on_radioButtonPontoQualquer_toggled), NULL);
    g_signal_connect(buttonCancelarRotacao, "button-release-event", G_CALLBACK (on_buttonCancelarRotacao_clicked), NULL);
    g_signal_connect(buttonSalvarRotacao, "button-release-event", G_CALLBACK (on_buttonSalvarRotacao_clicked), NULL);

    g_signal_connect(buttonCancelarEdicao, "button-release-event", G_CALLBACK (on_buttonCancelarEdicao_clicked), NULL);
    g_signal_connect(buttonSalvarEdicao, "button-release-event", G_CALLBACK (on_buttonSalvarEdicao_clicked), NULL);
    g_signal_connect(radioButtonTransladar, "toggled", G_CALLBACK(on_radioButtonTransladar_toggled), NULL);

    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(drawingArea, "configure-event", G_CALLBACK(configure_event_cb), NULL);

    g_signal_connect(windowPrincipal, "destroy", G_CALLBACK(on_buttonFecharWindowPrincipal), NULL);

    gtk_widget_add_events(windowPrincipal, GDK_KEY_PRESS_MASK);
    g_signal_connect(windowPrincipal, "key-release-event", G_CALLBACK(on_qualquerTeclaTeclado_Pressed),NULL);

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(windowPrincipal);                
    monstrarMensagemNoConsole("Pressione o botao F1 para obter ajuda e/ou ler a documentação.\n");

    gtk_main();

    return 0;
}
