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
#ifndef ITRAKTTASK
    #include "ITraktTask.cpp"
    #define ITRAKTTASK
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
GtkWidget *gWindowGrid = 0;
GtkWidget *gStatGrid = 0;

struct ControlStat{
    GtkWidget *label = 0;
    GtkWidget *amountLabel = 0;
    long long amount = 0;
    ITraktTask::StatisticItem *item = 0;
    ControlStat *next = 0;
    ControlStat *previous = 0;
};

ControlStat *gAllControlStats = 0;
ControlStat *gLastControlStat = 0;

int gCurrentRow = 2;

Logger gLogger = Logger("MAIN");

void showReturnHouseKeepMenu(GtkButton *pButton, gpointer pUser_data);
void showHouseKeepMenu(GtkButton *pButton, gpointer pUser_data);
void setupWaitImage();
//void showHousekeepRatings(GtkButton *pButton, gpointer pUser_data);