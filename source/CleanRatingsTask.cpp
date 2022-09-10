#ifndef ITRAKTTASK
    #include "ITraktTask.cpp"
    #define ITRAKTTASK
#endif
class CleanRatingsTask : public ITraktTask {
    public:
        bool run(){
            initialise();
            if(gCurrentRating % 2==1){
                getResults();
            }else{
                deleteResults();
            }
            updateStage();
            return false;
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
            lSS << getStageName() << "/" << ((gCurrentRating + 1)/ 2) << "?page=1&limit=10";

            long lStatusCode = 0;
            std::string lPage = gPageManager.getPage(lSS.str().c_str(), &lStatusCode);
            gJson = new JsonBuilder;
            gJson->parseString(lPage.c_str());
            gLog.log(gJson->getFormatted());

        }

        void deleteResults(){
            gLog.log("delete result", gCurrentRating / 2);
        }

        void updateStage(){
            gCurrentRating++;
            if(gCurrentRating>20){
                gCurrentRating = 1;
                gCurrentStage++;
            }
        }

        bool isRunning(){
            return !isStopped();
        };

        bool isStopped(){
            if(gStopProcessing){
                return true;
            }
            return gCurrentStage > STAGE_ALL;
        };

        long long getTasksDone(){
            return ((gCurrentStage + 1) * 20) + (gCurrentRating + 1);
        };

        long long getTotalTasks(){
            return ((STAGE_ALL + 1) * 20) + 20;
        };

        CleanRatingsTask(const char *pClientID, const char *pApiVersion, const char *pAccessToken){
            gPageManager.setDefaultHeaderDetails(pClientID, pApiVersion, pAccessToken);

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
