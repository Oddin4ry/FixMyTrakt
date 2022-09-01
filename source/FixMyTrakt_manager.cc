#include "DBManager.cc"
#include "FixMyTrakt_httpstraktmanager.cc"
#include <string>
#ifndef JSON_BUILDER
#include "jsonbuilder.cpp"
#define JSON_BUILDER
#endif

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER

#endif
class FixMyTrakt_manager{
    DBManager gDB;
    const char *CLIENT_ID = "3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07";
    const char *CLIENT_SECRET = "784600aa1a6cb70c3dc84204689640efb569ff6a02adaf4f7929b6938fe6be21";
    std::string gDevice_code = "";
    std::string gAccessToken = "";

    public:
        bool initialise(){
            return true;
        }

        void getToken(){
            JsonBuilder lBuilder;
            lBuilder.setStringValue(lBuilder.newEntity("code"), gDevice_code.c_str());
            lBuilder.setStringValue(lBuilder.newEntity("client_id"), CLIENT_ID);
            lBuilder.setStringValue(lBuilder.newEntity("client_secret"), CLIENT_SECRET);
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

        void getAllRatings(){
            
        }

        std::string setupAuthenticationPage(){
            gDB.doRead(gDB.DB_TOKENS);
            gAccessToken = gDB.getStringValue("access_token");
            if(gAccessToken!=""){
                return "";
            }

            JsonBuilder lBuilder;
            lBuilder.setStringValue(lBuilder.newEntity("client_id"), CLIENT_ID);
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

};