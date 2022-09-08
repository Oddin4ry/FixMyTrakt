#ifndef HTTPS_TRAKT
#include "FixMyTrakt_httpstraktmanager.cc"
#define HTTPS_TRAKT
#endif

class ITraktTask{
    public:
        virtual bool run() = 0;
        virtual bool isRunning() = 0;
        virtual bool isStopped() = 0;
        virtual long long getTasksDone() = 0;
        virtual long long getTotalTasks() = 0;
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
            return true;
        };

        bool isStopped(){
            return gCurrentStage > STAGE_ALL;
        };

        long long getTasksDone(){
            return (gCurrentStage + 1) * (gCurrentRating + 1);
        };

        long long getTotalTasks(){
            return STAGE_ALL * STAGE_ALL;
        };

        CleanRatingsTask(const char *pClientID, const char *pApiVersion, const char *pAccessToken){
            gPageManager.setDefaultHeaderDetails(pClientID, pApiVersion, pAccessToken);
           // std::thread lThread( [this] { this->run(); }); // <-- crashes as well
         //   std::thread lThread(&CleanRatingsTask::run, this); <-- compiles but crashes
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
};
/*            /*FixMyTrakt_httpstraktmanager lPageManager;
            long lStatusCode = 0;
            setupPageManager(&lPageManager);
            std::string lPage = lPageManager.getPage("https://api.trakt.tv/sync/ratings", &lStatusCode);
            //std::string lPage = lPageManager.getPage("https://www.google.com", &lStatusCode);
            //std::string lPage = lPageManager.postPage("https://apitrakt.tv/sync/ratings", "", &lStatusCode);
            gLogger.log("PAGE", lPage);
            gLogger.log("STATUS CODE", lStatusCode);*/