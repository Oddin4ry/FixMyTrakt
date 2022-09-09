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

        Logger gLog = Logger("Task");
        StatisticItem *addStatistic(std::string pName){
            StatisticItem *lStat = gAllStatistics;
            while(lStat!=0){
                if(pName==lStat->name){
                    lStat->amount++;
                    return lStat;
                }
                lStat = lStat->next;
            }
            lStat = new StatisticItem;
            lStat->name = pName;
            lStat->amount = 1;
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

        StatisticItem *addStatistic(StatisticItem *pParent, std::string pName){
            if(pParent==0){
                return addStatistic(pName);
            }
            StatisticItem *lStat = pParent->allChildStatistics;
            while(lStat!=0){
                if(lStat->name==pName){
                    lStat->amount++;
                    pParent->amount++;
                    return lStat;
                }
                lStat = lStat->next;
            }

            lStat = new StatisticItem;
            lStat->name = pName;
            lStat->parentStatistic = pParent;
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
class CleanRatingsTask : public ITraktTask {
    public:
        bool run(){
            initialise();
            getResults();
            deleteResults();
            updateStage();
            return true;
        };

        std::string getTaskName(){
            return "Clean Ratings";
        }

        std::string getStageName(){
            if(gCurrentStage==STAGE_ALL){
                return "all";
            }else if(gCurrentStage==STAGE_EPISODES){
                return "episodes";
            }else if(gCurrentStage==STAGE_SEASONS){
                return "seasons";
            }else if(gCurrentStage==STAGE_SHOWS){
                return "shows";
            }else if(gCurrentStage==STAGE_MOVIES){
                return "movies";
            }
            return "ERROR";
        }

        void getResults(){
            std::stringstream lSS;
            lSS << "https://api.trakt.tv/sync/ratings/";
            lSS << getStageName() << "/" << gCurrentRating << "?page=1&limit=10";

            long lStatusCode = 0;
            std::string lPage = gPageManager.getPage(lSS.str().c_str(), &lStatusCode);
            gJson = new JsonBuilder;
            gJson->parseString(lPage.c_str());
            gLog.log(gJson->getFormatted());

        }

        void deleteResults(){

        }

        void updateStage(){
            gCurrentRating++;
            if(gCurrentRating>10){
                gCurrentRating = 1;
                gCurrentStage++;
            }
        }

        bool isRunning(){
            return !isStopped();
        };

        bool isStopped(){
            return gCurrentStage > STAGE_ALL;
        };

        long long getTasksDone(){
            return ((gCurrentStage + 1) * 10) + (gCurrentRating + 1);
        };

        long long getTotalTasks(){
            return (STAGE_ALL * 10) + 10;
        };

        CleanRatingsTask(const char *pClientID, const char *pApiVersion, const char *pAccessToken){
            gPageManager.setDefaultHeaderDetails(pClientID, pApiVersion, pAccessToken);
           // std::thread lThread( [this] { this->run(); }); // <-- crashes as well
         //   std::thread lThread(&CleanRatingsTask::run, this); <-- compiles but crashes
        };

        ~CleanRatingsTask(){
            if(gJson!=0){
                delete gJson;
            }
        };
    private:
        FixMyTrakt_httpstraktmanager gPageManager;
        short gCurrentRating = 0;
        short gCurrentStage = -1;
        const short STAGE_SHOWS = 0;
        const short STAGE_MOVIES = 1;
        const short STAGE_SEASONS = 2;
        const short STAGE_EPISODES = 3;
        const short STAGE_ALL = 4;
        
        void initialise(){
            if(gCurrentRating==0){
                gCurrentStage = 0;
                gCurrentRating = 1;
            }
        };
        JsonBuilder *gJson = 0;
};
/*            /*FixMyTrakt_httpstraktmanager lPageManager;
            long lStatusCode = 0;
            setupPageManager(&lPageManager);
            std::string lPage = lPageManager.getPage("https://api.trakt.tv/sync/ratings", &lStatusCode);
            //std::string lPage = lPageManager.getPage("https://www.google.com", &lStatusCode);
            //std::string lPage = lPageManager.postPage("https://apitrakt.tv/sync/ratings", "", &lStatusCode);
            gLogger.log("PAGE", lPage);
            gLogger.log("STATUS CODE", lStatusCode);*/