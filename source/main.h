#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/application.h>
#include "FixMyTrakt_manager.cc"
#include <iostream>
#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif



FixMyTrakt_manager gManager;
GtkWidget *gMainWindow;
GtkWidget *gMenuButtons;
GtkWidget *gWindowPane;
GtkWidget *gHouseKeepButton;
GtkWidget *gBackButton;
GtkWidget *gHouseKeepRatings;
GtkWidget *gProgress = 0;
GtkWidget *gStopProcess = 0;
Logger gLogger = Logger("MAIN");

void showReturnHouseKeepMenu(GtkButton *pButton, gpointer pUser_data);
void showHouseKeepMenu(GtkButton *pButton, gpointer pUser_data);
//void showHousekeepRatings(GtkButton *pButton, gpointer pUser_data);