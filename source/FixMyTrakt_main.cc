#include "main.h"

void stopProcess(GtkButton *pButton, gpointer pUser_data){
    gManager.stopProcess();
    gtk_widget_set_sensitive(gStopProcess, false);
}

ControlStat *newControlStat(ITraktTask::StatisticItem *pItem){
    ControlStat *lStat = new ControlStat;
    lStat->amount = pItem->amount;
    lStat->item = pItem;
    std::stringstream lSS;
    GtkWidget *lLabel = 0;
    if(pItem->parentStatistic!=0){
        lSS << " \\" << pItem->name;
        lLabel = gtk_label_new(lSS.str().c_str());
        lSS.str("");
        lSS << "<span foreground='lavenderblush'> -" << pItem->name << "</span>";
        gtk_label_set_markup(GTK_LABEL(lLabel), lSS.str().c_str());        
    }else{
        lLabel = gtk_label_new(pItem->name.c_str());
        lSS << "<span foreground='lightpink'>" << pItem->name << "</span>";
        gtk_label_set_markup(GTK_LABEL(lLabel), lSS.str().c_str());        
    }
    gtk_label_set_xalign(GTK_LABEL(lLabel), 0);
    gtk_grid_attach(GTK_GRID(gStatGrid), lLabel, 0, gCurrentRow, 2, 1);
    gtk_widget_show(lLabel);
    lStat->label = lLabel;

    lSS.str("");
    lSS << pItem->amount;
    lLabel = gtk_label_new(lSS.str().c_str());
    lSS.str("");
    
    if(pItem->parentStatistic!=0){
        lSS << " \\" << pItem->name;
        lLabel = gtk_label_new(lSS.str().c_str());
        lSS.str("");
        lSS << "<span foreground='lavenderblush'> -" << pItem->amount << "</span>";
    }else{
        lLabel = gtk_label_new(pItem->name.c_str());
        lSS << "<span foreground='lightpink'>" << pItem->amount << "</span>";
    }
    gtk_label_set_markup(GTK_LABEL(lLabel), lSS.str().c_str());
    gtk_label_set_xalign(GTK_LABEL(lLabel), 1);
    gtk_grid_attach(GTK_GRID(gStatGrid), lLabel, 2, gCurrentRow, 1, 1);

    lStat->amountLabel = lLabel;
    lStat->next = 0;
    lStat->previous = 0;
    gCurrentRow++;
    gtk_widget_show(lLabel);
    gtk_widget_show(gStatGrid);
    if(gAllControlStats==0){
        gAllControlStats = lStat;
    }
    if(gLastControlStat!=0){
        gLastControlStat->next = lStat;
        lStat->previous = gLastControlStat;
    }
    gLastControlStat = lStat;
    return lStat;
}

void destroyStats(){
    ControlStat *lStat = gAllControlStats;
    if(lStat!=0){

        ControlStat *bNext = lStat->next;
        delete lStat;
        lStat = bNext;
    }
    gAllControlStats = 0;
}

void updateControlStat(ControlStat *pControlStat){
    std::stringstream lSS;
    if(pControlStat->item->parentStatistic!=0){
        lSS << "<span foreground='lavenderblush'> -" << pControlStat->item->amount << "</span>";
    }else{
        lSS << "<span foreground='lightpink'>" << pControlStat->item->amount << "</span>";
    }
    gtk_label_set_markup(GTK_LABEL(pControlStat->amountLabel), lSS.str().c_str());
}

void deleteControlStatsFrom(ControlStat *pControlStat){
    ControlStat *lStat = pControlStat->next;
    if(lStat==0){
        return;
    }
    gLastControlStat = lStat->previous;
    if(lStat->previous!=0){
        lStat->previous->next = 0;
    }
    if(gAllControlStats==lStat){
        gAllControlStats = 0;
    }
    while(lStat!=0){
        ControlStat *bNext = lStat->next;
        gtk_widget_destroy(lStat->label);
        gtk_widget_destroy(lStat->amountLabel);
        gCurrentRow--;        
        delete lStat;
        lStat = bNext;
    }
    pControlStat = 0;

}

ControlStat *updateChildStats(ITraktTask::StatisticItem *pParent, ControlStat *pCurrentControlStat){
    ITraktTask::StatisticItem *lChild = pParent->allChildStatistics;
    while(lChild!=0){
        if(pCurrentControlStat==0){
            pCurrentControlStat = newControlStat(lChild);
        }else if(pCurrentControlStat->item==lChild){
            updateControlStat(pCurrentControlStat);
        }else{
            deleteControlStatsFrom(pCurrentControlStat);
            pCurrentControlStat = newControlStat(lChild);
            
        }
        if(pCurrentControlStat!=0){
            pCurrentControlStat = pCurrentControlStat->next;
        }
        lChild = lChild->next;
    }
    return pCurrentControlStat;

}

void updateStats(){
    if(gManager.getFirstStat()==0){
        return;
    }
    ITraktTask::StatisticItem *lItem = gManager.getFirstStat();
    ControlStat *lControlStat = gAllControlStats;
    while(lItem!=0){
        if(lControlStat==0){
            lControlStat = newControlStat(lItem);
        }else if(lControlStat->item==lItem){
            updateControlStat(lControlStat);
        }else{
            deleteControlStatsFrom(lControlStat);
            lControlStat = newControlStat(lItem);
        }
        if(lControlStat!=0){
            lControlStat = lControlStat->next;
        }
        if(lItem->allChildStatistics!=0){
            lControlStat = updateChildStats(lItem, lControlStat);
        }
        lItem = lItem->next;
    }
}

void closeStatsWindow(GtkButton *pButton, gpointer pUser_data){
    gtk_widget_destroy((GtkWidget*) pUser_data);
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

    updateStats();

    if(lContinue){
        return G_SOURCE_CONTINUE;
    }
    gtk_widget_destroy(gStopProcess);
    //gtk_widget_destroy(gWindowGrid);
    gtk_widget_set_sensitive(gMenuButtons, true);
    
    GtkWidget *lCloseButton = gtk_button_new_with_label("Close");
    gtk_grid_attach(GTK_GRID(gStatGrid),lCloseButton, 0, gCurrentRow++, 3, gCurrentRow);
    g_signal_connect(G_OBJECT (lCloseButton), "clicked", G_CALLBACK(closeStatsWindow), gWindowGrid);
    gtk_widget_show_all(gStatGrid);
    destroyStats();
    return G_SOURCE_REMOVE;
}

void setupProgress(){
    gWindowGrid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(gWindowGrid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(gWindowGrid), 10);
    gtk_container_add(GTK_CONTAINER(gWindowPane), gWindowGrid);
    gtk_widget_show(gWindowGrid);

    gProgress = gtk_progress_bar_new();
    gtk_grid_attach(GTK_GRID(gWindowGrid), gProgress, 0, 0, 2, 1);
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(gProgress), true);
    gStopProcess = gtk_button_new_with_label("Stop");
    gtk_grid_attach(GTK_GRID(gWindowGrid), gStopProcess, 2, 0, 1, 1);

    gStatGrid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(gStatGrid), 5);
    gtk_grid_attach(GTK_GRID(gWindowGrid), gStatGrid, 1, 1, 3, 1);

    GtkWidget *lLabel = gtk_label_new("STATISTIC NAME");
    gtk_grid_attach(GTK_GRID(gStatGrid), lLabel, 0, 1, 2, 1);
    gtk_label_set_markup(GTK_LABEL(lLabel), "<span background='indigo' foreground='hotpink'><b>Statistic</b></span>");

    lLabel = gtk_label_new("AMOUNT");
    gtk_grid_attach(GTK_GRID(gStatGrid), lLabel, 2, 1, 1, 1);
    gtk_label_set_markup(GTK_LABEL(lLabel), "<span background='indigo' foreground='hotpink'><b>Amount</b></span>");
    gtk_label_set_xalign(GTK_LABEL(lLabel), 1);
    gtk_widget_show(gStatGrid);
    gCurrentRow = 2;
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
        destroyStats();
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