using namespace std;
#include <gtk/gtk.h>
#include <iostream>

    GtkWidget *window;

    // Botões da caixa de controle
    GtkWidget *buttonZoomIn;
    GtkWidget *buttonZoomOut;
    GtkWidget *buttonCima;
    GtkWidget *buttonBaixo;
    GtkWidget *buttonEsquerda;
    GtkWidget *buttonDireita;

    // Botões dos objetos a serem desenhados
    GtkWidget *buttonPonto;
    GtkWidget *buttonReta;
    GtkWidget *buttonPoligono;

    // Área de desenho
    GtkWidget *drawingArea;
    
    // Console e objetos
    GtkWidget *textConsole;
    GtkWidget *objectsList;
    GtkScrolledWindow *scrolledConsole;

// chama este método quando o botão baixo é clicado
static void on_buttonBaixo_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão baixo pressionado!\n", -1);

}

// chama este método quando o botão cima é clicado
static void on_buttonCima_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão cima pressionado!\n", -1);

}

// chama este método quando o botão esquerda é clicado
static void on_buttonEsquerda_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão esquerda pressionado!\n", -1);

}

// chama este método quando o botão direita é clicado
static void on_buttonDireita_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão direita pressionado!\n", -1);

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

}

// chama este método quando o botão poligono é clicado
static void on_buttonPoligono_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão poligono pressionado!\n", -1);
    
}

// chama este método quando o botão ponto é clicado
static void on_buttonPonto_clicked() {
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textConsole));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Botão ponto pressionado!\n", -1);

}

int main(int argc, char *argv[]) {
   
    GtkBuilder *builder;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "cg_ui.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "windowPrincipal"));

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

    g_signal_connect(buttonBaixo, "button-release-event", G_CALLBACK (on_buttonBaixo_clicked), NULL);
    g_signal_connect(buttonCima, "button-release-event", G_CALLBACK (on_buttonCima_clicked), NULL);
    g_signal_connect(buttonEsquerda, "button-release-event", G_CALLBACK (on_buttonEsquerda_clicked), NULL);
    g_signal_connect(buttonDireita, "button-release-event", G_CALLBACK (on_buttonDireita_clicked), NULL);
    g_signal_connect(buttonZoomIn, "button-release-event", G_CALLBACK (on_buttonZoomIn_clicked), NULL);
    g_signal_connect(buttonZoomOut, "button-release-event", G_CALLBACK (on_buttonZoomOut_clicked), NULL);
    g_signal_connect(buttonPonto, "button-release-event", G_CALLBACK (on_buttonPonto_clicked), NULL);
    g_signal_connect(buttonPoligono, "button-release-event", G_CALLBACK (on_buttonPoligono_clicked), NULL);
    g_signal_connect(buttonReta, "button-release-event", G_CALLBACK (on_buttonReta_clicked), NULL);
    
    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);                
    gtk_main();


    return 0;
}
