#include "DBManager.cc"
#include "FixMyTrakt_httpstraktmanager.cc"
#include <string>
#include "jsonbuilder.cpp"

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER

#endif
class FixMyTrakt_manager{
    const char *CLIENT_ID = "3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07";
    const char *CLIENT_SECRET = "784600aa1a6cb70c3dc84204689640efb569ff6a02adaf4f7929b6938fe6be21";
    std::string gDevice_code = "";
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
            gLogger.log("STATUS CODE", lStatusCode);
        }

        std::string setupAuthenticationPage(){
             /*std::string lURL = "xdg-open ";
             lURL.append("\"https://api.trakt.tv/oauth/authorize?response_type=code&client_id=3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07&redirect_uri=urn:ietf:wg:oauth:2.0:oob\"");
             system(lURL.c_str());*/
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