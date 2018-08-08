#include <gtk/gtk.h>

GtkWidget *g_label_hello;
GtkWidget *g_label_count;

int main(int argc, char *argv[]) {
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    // ponteiros para as labels
    g_label_hello = GTK_WIDGET(gtk_builder_get_object(builder, "label_hello"));
    g_label_count = GTK_WIDGET(gtk_builder_get_object(builder, "label_count"));


    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

// chama este método quando o botão hello é clicado
void on_button_hello_clicked() {
    static unsigned int count = 0;
    char str_count [30] = {0};

    gtk_label_set_text(GTK_LABEL(g_label_hello), "Hello World!");
    count++;
    sprintf(str_count, "%d", count);
    gtk_label_set_text(GTK_LABEL(g_label_count), str_count);
}

// chama este método quando a janela é fechada
void on_window_main_destroy() {
    gtk_main_quit();
}
