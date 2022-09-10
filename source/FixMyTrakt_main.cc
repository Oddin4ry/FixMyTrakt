#include "main.h"

void stopProcess(GtkButton *pButton, gpointer pUser_data){
    gManager.stopProcess();
    gtk_widget_set_sensitive(gStopProcess, false);
}

gboolean pollProgress(gpointer user_data){
    bool lIsRunning = false;
    long long lTotalTasks = 0;
    long long lCurrentTasks = 0;
    std::string lTaskName = gManager.getTaskName();
    bool lContinue = gManager.pollTask(lIsRunning, lCurrentTasks, lTotalTasks);
    
    double lPercent = (double)lCurrentTasks / (double)lTotalTasks;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gProgress), lPercent);
    std::stringstream lSS;
    lSS << lTaskName << " " << lPercent;
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(gProgress), lSS.str().c_str());
    
    if(lContinue){
        return G_SOURCE_CONTINUE;
    }
    gtk_widget_destroy(gProgress);
    gtk_widget_destroy(gStopProcess);
    gtk_widget_set_sensitive(gMenuButtons, true);
    return G_SOURCE_REMOVE;
}

void setupProgress(){
    gProgress = gtk_progress_bar_new();
    gtk_container_add(GTK_CONTAINER(gWindowPane), gProgress);
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(gProgress), true);
    gStopProcess = gtk_button_new_with_label("Stop");
    gtk_container_add(GTK_CONTAINER(gWindowPane), gStopProcess);
    g_signal_connect(G_OBJECT (gStopProcess), "clicked", G_CALLBACK(stopProcess), NULL);      
    gtk_widget_show_all(gMainWindow); 
    g_timeout_add(100, pollProgress, 0);
    
}
void showHousekeepRatings(GtkButton *pButton, gpointer pUser_data){
    gtk_widget_set_sensitive(gMenuButtons,false);
    gManager.getAllRatings();
    setupProgress();
    
}

void clearMainMenu(){
    gtk_widget_destroy(gHouseKeepButton);
}
void clearHouseKeepMenu(){
    gtk_widget_destroy(gBackButton);
    gtk_widget_destroy(gHouseKeepRatings);
}

void showMainMenu(){
    gHouseKeepButton = gtk_button_new_with_label("House Keeping");
    g_signal_connect(G_OBJECT (gHouseKeepButton), "clicked", G_CALLBACK(showHouseKeepMenu), NULL);
    gtk_box_pack_start(GTK_BOX(gMenuButtons), gHouseKeepButton, 0, 0, 0);
    gtk_widget_show_all(gMainWindow);
}


void showReturnHouseKeepMenu(GtkButton *pButton, gpointer pUser_data){
    clearHouseKeepMenu();
    showMainMenu();
}


void showHouseKeepMenu(GtkButton *pButton, gpointer pUser_data){
    clearMainMenu();
    gBackButton = gtk_button_new_with_label("Home");
    g_signal_connect(G_OBJECT (gBackButton), "clicked", G_CALLBACK(showReturnHouseKeepMenu), NULL);
    gtk_box_pack_start(GTK_BOX(gMenuButtons), gBackButton, 0, 0, 0);
    gHouseKeepRatings = gtk_button_new_with_label("House Keep Ratings");
    g_signal_connect(G_OBJECT (gHouseKeepRatings), "clicked", G_CALLBACK(showHousekeepRatings), NULL);
    gtk_box_pack_start(GTK_BOX(gMenuButtons), gHouseKeepRatings, 0, 0, 0);
    gtk_widget_show_all(gMainWindow);    
}



static void destroy(GtkWidget *pWidget, gpointer pData){
    if (pWidget==gMainWindow){
        gtk_main_quit();
    }
}
void authentication_clicked(GtkButton *pButton, gpointer user_data){
    gManager.getToken();
}

void showAuthenticationCode(std::string pPin){
    GtkWidget *lAuthenticationWindow;
    lAuthenticationWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(lAuthenticationWindow), "Verify");
    gtk_window_set_default_size(GTK_WINDOW(lAuthenticationWindow),300, 300);
    gtk_window_set_modal(GTK_WINDOW(lAuthenticationWindow), true);
    gtk_window_set_position(GTK_WINDOW(lAuthenticationWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_keep_above(GTK_WINDOW(lAuthenticationWindow), true);

    GtkWidget *lGrid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(lGrid), 10);

    gtk_container_add(GTK_CONTAINER(lAuthenticationWindow), lGrid);
    gtk_widget_show(lGrid);

    GtkWidget *lLabel = gtk_label_new("Pin");
    gtk_grid_attach(GTK_GRID(lGrid), lLabel, 0, 0, 1, 1);
    gtk_widget_show(lLabel);

    GtkWidget *lPinIn = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(lPinIn), pPin.c_str());
    gtk_editable_set_editable(GTK_EDITABLE(lPinIn), FALSE);;

    gtk_grid_attach(GTK_GRID(lGrid), lPinIn, 1, 0, 1, 1);
    gtk_widget_show(lPinIn);


    GtkWidget *lButton = gtk_button_new_with_label("Authenticated");
    gtk_grid_attach(GTK_GRID(lGrid), lButton, 0, 1, 2, 1);
    gtk_widget_show(lButton);
    g_signal_connect(G_OBJECT (lButton), "clicked", G_CALLBACK(authentication_clicked), NULL);

    gtk_widget_show(lAuthenticationWindow);

}

void initialise(){
    if(gManager.initialise()){
        // We are authenticating
        
    }
    std::string lDeviceCode = gManager.setupAuthenticationPage();
    if(lDeviceCode!=""){

        showAuthenticationCode(lDeviceCode);
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
    gtk_window_resize(GTK_WINDOW(gMainWindow), lWorkArea.width, lWorkArea.height);

    GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gMenuButtons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vBox), gMenuButtons,false,true,10);
    gWindowPane = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vBox), gWindowPane,false,true,10);
    gtk_container_add(GTK_CONTAINER(gMainWindow), vBox);

    gtk_widget_show_all(gMainWindow);

}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    initialise();
    setupMainWindow();
    showMainMenu();
    gtk_widget_show(gMainWindow);
    gtk_main();
    return 0;

}