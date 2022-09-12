#ifndef HTTPS_TRAKT
#include "FixMyTrakt_httpstraktmanager.cc"
#define HTTPS_TRAKT
#endif

#ifndef JSON_BUILDER
#include "jsonbuilder.cpp"
#define JSON_BUILDER
#endif

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif

class ITraktTask{
    public:
        virtual bool run() = 0;
        virtual bool isRunning() = 0;
        virtual bool isStopped() = 0;
        virtual long long getTasksDone() = 0;
        virtual long long getTotalTasks() = 0;
        virtual std::string getTaskName() = 0;

        struct StatisticItem{
            std::string name = "";
            long long unsigned amount = 0;
            StatisticItem *allChildStatistics = 0;
            StatisticItem *parentStatistic = 0;
            StatisticItem *next = 0;
            StatisticItem *lastChild = 0;
            StatisticItem *previous = 0;
        };

        StatisticItem *getFirstStat(){
            return gAllStatistics;
        }



        Logger gLog = Logger("Task");
        StatisticItem *addStatistic(std::string pName, long long pAmount){
            StatisticItem *lStat = gAllStatistics;
            while(lStat!=0){
                if(pName==lStat->name){
                    lStat->amount = lStat->amount + pAmount;
                    return lStat;
                }
                lStat = lStat->next;
            }
            lStat = new StatisticItem;
            lStat->name = pName;
            lStat->amount = pAmount;
            lStat->allChildStatistics = 0;
            if(gAllStatistics==0){
                gAllStatistics = lStat;
            }
            if(gLast!=0){
                gLast->next = lStat;
                lStat->previous = gLast;
            }
            gLast = lStat;
            return lStat;
        };

        bool gStopProcessing = false;

        void stop(){
            gStopProcessing = true;
        }

        StatisticItem *addStatistic(StatisticItem *pParent, std::string pName, long long pAmount){
            if(pParent==0){
                return addStatistic(pName, pAmount);
            }
            pParent->amount = pParent->amount + pAmount;
            StatisticItem *lStat = pParent->allChildStatistics;
            while(lStat!=0){
                if(lStat->name==pName){
                    lStat->amount = lStat->amount + pAmount;
                    return lStat;
                }
                lStat = lStat->next;
            }

            lStat = new StatisticItem;
            lStat->name = pName;
            lStat->parentStatistic = pParent;
            lStat->amount = pAmount;
            if(pParent->allChildStatistics==0){
                pParent->allChildStatistics = lStat;
            }
            if(pParent->lastChild!=0){
                pParent->lastChild->next = lStat;
                lStat->previous = pParent->lastChild;
            }
             pParent->lastChild = lStat;
            return lStat;
        };        

    private:
       



        StatisticItem *gAllStatistics = 0;
        StatisticItem *gLast = 0;

};
