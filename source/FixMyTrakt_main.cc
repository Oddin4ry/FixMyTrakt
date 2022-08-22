#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/application.h>
#include "FixMyTrakt_manager.cc"
#include <iostream>

FixMyTrakt_manager gManager;

static void destroy(GtkWidget *pWidget, gpointer pData){
    gtk_main_quit();
}

void initialise(){
    gManager.initialise();
}

int main(int argc, char *argv[]){
    GtkWidget *lWindow;
    gtk_init(&argc, &argv);
    lWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_icon_theme_append_search_path(gtk_icon_theme_get_default(), "/home/james/source/fixmytrakt/FixMyTrakt/icons/");
    g_set_application_name("Fix My Trakt");
    
    gtk_window_set_title(GTK_WINDOW(lWindow), "Fix My Trakt");
    g_signal_connect(G_OBJECT (lWindow), "destroy", G_CALLBACK(destroy), NULL);
    initialise();
    gtk_widget_show(lWindow);
    initialise();
    gtk_main();
    return 0;

}