#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/application.h>
#include "FixMyTrakt_manager.cc"
#include <iostream>

FixMyTrakt_manager gManager;
GtkWidget *gMainWindow;

static void destroy(GtkWidget *pWidget, gpointer pData){
    if (pWidget==gMainWindow){
        gtk_main_quit();
    }
}
/*void authentication_clicked(GtkButton *pButton, gpointer user_data){
    std::string lPin = gtk_entry_get_text(GTK_ENTRY(user_data));
    gManager.getToken(lPin.c_str());
    //std::cout << "AUTHENTICATION CLICKED" << gtk_entry_get_text(GTK_ENTRY(user_data));
}*/

/*void setupAuthenticationPage(){
    GtkWidget *lAuthenticationWindow;
    lAuthenticationWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(lAuthenticationWindow), "Enter Trakt Pin");
    gtk_window_set_default_size(GTK_WINDOW(lAuthenticationWindow),300, 300);
    gtk_window_set_modal(GTK_WINDOW(lAuthenticationWindow), true);
    gtk_window_set_position(GTK_WINDOW(lAuthenticationWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_keep_above(GTK_WINDOW(lAuthenticationWindow), true);

    GtkWidget *lGrid = gtk_grid_new();

    gtk_container_add(GTK_CONTAINER(lAuthenticationWindow), lGrid);
    gtk_widget_show(lGrid);

    GtkWidget *lLabel = gtk_label_new("Pin");
    gtk_grid_attach(GTK_GRID(lGrid), lLabel, 0, 0, 1, 1);
    gtk_widget_show(lLabel);

    GtkWidget *lPinIn = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(lPinIn), 8);
    gtk_grid_attach(GTK_GRID(lGrid), lPinIn, 1, 0, 1, 1);
    gtk_widget_show(lPinIn);

    GtkWidget *lButton = gtk_button_new_with_label("Authenticate");
    gtk_grid_attach(GTK_GRID(lGrid), lButton, 0, 1, 2, 1);
    gtk_widget_show(lButton);
    g_signal_connect(G_OBJECT(lButton), "clicked", G_CALLBACK(authentication_clicked), lPinIn);
    gtk_widget_show(lAuthenticationWindow);
}
*/

void initialise(){
    if(gManager.initialise()){
        // We are authenticating
        
    }
    std::string lDeviceCode = gManager.setupAuthenticationPage();
    if(lDeviceCode!=""){

    }
}

void setupMainWindow(){
    GdkRectangle lWorkArea = {0};
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &lWorkArea);
    gMainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_icon_theme_append_search_path(gtk_icon_theme_get_default(), "/home/james/source/fixmytrakt/FixMyTrakt/icons/");
    g_set_application_name("Fix My Trakt");

    gtk_window_set_title(GTK_WINDOW(gMainWindow), "Fix My Trakt");
    g_signal_connect(G_OBJECT (gMainWindow), "destroy", G_CALLBACK(destroy), NULL);
    gtk_window_set_position(GTK_WINDOW(gMainWindow), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_resize(GTK_WINDOW(gMainWindow), lWorkArea.width, lWorkArea.height);

}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    initialise();
    setupMainWindow();
    gtk_widget_show(gMainWindow);
    gtk_main();
    return 0;

}