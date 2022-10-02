#include "DBManager.cc"
#ifndef HTTPS_TRAKT
#include "FixMyTrakt_httpstraktmanager.cc"
#define HTTPS_TRAKT
#endif

#include <string>
#ifndef JSON_BUILDER
#include "jsonbuilder.cpp"
#define JSON_BUILDER
#endif

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif

#ifndef ITRAKTTASK
    #include "ITraktTask.cpp"
    #define ITRAKTTASK
#endif

#include "CleanRatingsTask.cpp"

class FixMyTrakt_manager{
    DBManager gDB;
    const char *CLIENT_ID = "3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07";
    const char *CLIENT_SECRET = "784600aa1a6cb70c3dc84204689640efb569ff6a02adaf4f7929b6938fe6be21";
    const char *API_VERSION = "2";
    bool gIsVIP = false;
    std::string gDevice_code = "";
    std::string gAccessToken = "";
    std::string gUserName =  "";
    std::string gName = "";
    std::string gUserID = "";
    std::string gAvatarPath = "";
    std::string gCoverPath = "";
    unsigned char *gAvatarBinary = 0;
    unsigned char *gBannerBinary = 0;
    long gAvatarBinaryLength = 0;
    long gBannarBinaryLength = 0;

    int gVIPYears = 0;

    public:
        bool initialise(){
            return true;
        }
        ~FixMyTrakt_manager(){
            if(gCurrentTask!=0){
                delete gCurrentTask;
            }
            if(gBannarBinaryLength>0){
                free(gBannerBinary);
            }
            if(gAvatarBinaryLength>0){
                free(gAvatarBinary);
            }
        };

        ITraktTask::StatisticItem *getFirstStat(){
            if(gCurrentTask!=0){
                return gCurrentTask->getFirstStat();
            }
            return 0;
        }

        void getToken(){
            JsonBuilder lBuilder;
            lBuilder.newValue("code", gDevice_code.c_str());
            lBuilder.newValue("client_id", CLIENT_ID);
            lBuilder.newValue("client_secret", CLIENT_SECRET);
            FixMyTrakt_httpstraktmanager lPageManger;
            long lStatusCode = 0;
            std::string lPage = lPageManger.postPage("https://api.trakt.tv/oauth/device/token", lBuilder.getFormatted().c_str(), &lStatusCode);
            if(lStatusCode==200){
                JsonBuilder bBuilder;
                bBuilder.parseString(lPage.c_str());
                gDB.beginUpdate(gDB.DB_TOKENS);
                gDB.setStringValue("access_token", bBuilder.getStringValue("access_token").c_str());
                gDB.setStringValue("expires_in", bBuilder.getStringValue("expires_in").c_str());
                gDB.setStringValue("token_type", bBuilder.getStringValue("token_type").c_str());
                gDB.setStringValue("refresh_token", bBuilder.getStringValue("refresh_token").c_str());
                gDB.setStringValue("created_at", bBuilder.getStringValue("created_at").c_str());

                gDB.endUpdate();
            }
        }

        void setupPageManager(FixMyTrakt_httpstraktmanager *pPageManager){
            pPageManager->setDefaultHeaderDetails(CLIENT_ID, API_VERSION, gAccessToken.c_str());
            pPageManager->gIsJSON = true;
        }

        void getSettings(){
            FixMyTrakt_httpstraktmanager lPageManager;
            setupPageManager(&lPageManager);
            long lStatusCode = 0;
            std::string lPage = lPageManager.getPage("https://api.trakt.tv/users/settings", &lStatusCode);
            JsonBuilder lJson;
            lJson.parseString(lPage.c_str());
            gLogger.log("IN", lJson.getFormatted());
            gLogger.log("Status", lStatusCode);
            Json::Value lUser = lJson.gRoot["user"];
            gIsVIP = lUser.get("vip", "").asBool();
            gUserName = lUser.get("username", "").asString();
            gName = lUser.get("name", "").asString();
            gUserID = lUser.get("ids","").get("slug", "").asString();
            gAvatarPath = lUser.get("images", "").get("avatar", "").get("full","").asString();
            gCoverPath = lUser.get("vip_cover_image","").asString();
            gVIPYears = lUser.get("vip_years", 0).asInt();
            gLogger.log(gUserID, gVIPYears);

            FixMyTrakt_httpstraktmanager lGfxPageManager;
            if(gAvatarBinaryLength>0){
                free(gAvatarBinary);
            }
            gAvatarBinary = lGfxPageManager.getPicture(gAvatarPath.c_str(), &lStatusCode, &gAvatarBinaryLength);
            if(lStatusCode!=200){
                gLogger.log("ERror gettinng avatar");
            }
            if(gBannarBinaryLength>0){
                free(gBannerBinary);
            }
            gBannerBinary = lGfxPageManager.getPicture(gCoverPath.c_str(), &lStatusCode, &gBannarBinaryLength);
            if(lStatusCode!=200){
                gLogger.log("ERror gettinng background");
            }
            gDBManager.beginUpdate("pictures");
            gDBManager.setBinaryValue("banner", gBannerBinary, gAvatarBinaryLength);
            gDBManager.setBinaryValue("avator", gAvatarBinary, gAvatarBinaryLength);
            gDBManager.endUpdate();
        }

        void getStats(){
        }

        void clearAllRatings(){
            if(gCurrentTask!=0){
                return;
            }
            gCurrentTask = new CleanRatingsTask(CLIENT_ID, API_VERSION, gAccessToken.c_str());
            gCurrentTask->run();

        }

        std::string getTaskName(){
            if(gCurrentTask==0){
                return "N/A";
            }
            return gCurrentTask->getTaskName();
        }

        void stopProcess(){
            if(gCurrentTask!=0){
                gCurrentTask->gStopProcessing = true;
            }
        }

        bool pollTask(bool &pRunning, long long &pProgressDone, long long &pTotalItems){
            if(gCurrentTask==0){
                pRunning = 0;
                pProgressDone = 1;
                pTotalItems = 1;
                gLogger.log("FIRST RETURN TRUE");
                return false;
            }
            pProgressDone = gCurrentTask->getTasksDone();
            pTotalItems = gCurrentTask->getTotalTasks();
            if(gCurrentTask->isRunning()){
                gCurrentTask->run();
                pProgressDone = gCurrentTask->getTasksDone();
                pTotalItems = gCurrentTask->getTotalTasks();
                pRunning = gCurrentTask->isRunning();                
            }else{
                pRunning = false;
               
            }
            if(!pRunning){
                delete gCurrentTask;
                gCurrentTask = 0;
            }
            return pRunning;
        }

        std::string setupAuthenticationPage(){
            gDB.doRead(gDB.DB_TOKENS);
            gAccessToken = gDB.getStringValue("access_token");
            if(gAccessToken!=""){
                return "";
            }

            JsonBuilder lBuilder;
            lBuilder.newValue("client_id", CLIENT_ID);
            FixMyTrakt_httpstraktmanager lPageManger;
            long lStatusCode = 0;
            std::string lPage = lPageManger.postPage("https://api.trakt.tv/oauth/device/code", lBuilder.getFormatted().c_str(), &lStatusCode);
            if(lStatusCode==200){
                JsonBuilder bBuilder;
                bBuilder.parseString(lPage.c_str());
                std::string lURL = "xdg-open ";
                lURL.append(bBuilder.getStringValue("verification_url"));
                system(lURL.c_str());

                gDevice_code = bBuilder.getStringValue("device_code");

                return bBuilder.getStringValue("user_code");
            }else{
                gLogger.log("Response", lStatusCode);
            }
            return "";
        }

    private:
        Logger gLogger = Logger("FixMTraktManager");
        DBManager gDBManager;
        ITraktTask *gCurrentTask = 0;

};
