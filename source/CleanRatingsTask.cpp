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
        std::string getStageNameSingular(){
            if(gCurrentStage==STAGE_ALL){
                return "all";
            }else if(gCurrentStage==STAGE_EPISODES){
                return "episode";
            }else if(gCurrentStage==STAGE_SEASONS){
                return "season";
            }else if(gCurrentStage==STAGE_SHOWS){
                return "show";
            }else if(gCurrentStage==STAGE_MOVIES){
                return "movie";
            }
            return "ERROR";
        }

        void getResults(){
            std::stringstream lSS;
            lSS << "https://api.trakt.tv/sync/ratings/";
            lSS << getStageName() << "/" << ((gCurrentRating + 1)/ 2) << "?page=1&limit=500";

            long lStatusCode = 0;
            std::string lPage = gPageManager.getPage(lSS.str().c_str(), &lStatusCode);
            gJson = new JsonBuilder;
            gJson->parseString(lPage.c_str());


        }

        void deleteResults(){
            if(gJson->isEmpty()){
                return;
            }
            gCurrentRating = gCurrentRating - 2;
            JsonBuilder *lNew = new JsonBuilder;
            std::stringstream lSS;
            lSS << getStageName();
            std::vector<Json::Value> lChildren = gJson->getAllChilden(getStageNameSingular());
            lNew->addJsonVector(getStageName().c_str(), lChildren);

            lSS.str("");
            lSS << "https://api.trakt.tv/sync/ratings/remove";

            long lStatusCode = 0;
            std::string lPage = gPageManager.postPage(lSS.str().c_str(), lNew->getFormatted().c_str(), &lStatusCode);
            if(lPage==""||lStatusCode!=200){
                gLog.log("ERROR:", lStatusCode);
            }else{
                lSS.str("");
                lSS << "Rating " << ((gCurrentRating + 1) / 2);
                if(gJson->getSize()==0){
                    addStatistic(getStageName(), 0);
                }else{
                    addStatistic(addStatistic(getStageName(), 0), lSS.str().c_str(), gJson->getSize());
                }            
            }
            delete lNew;
        }

        void updateStage(){
            gCurrentRating++;
            if(gCurrentRating>20){
                gCurrentStage++;
                gCurrentRating = 1;
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
