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
    public:
        bool initialise(){
            return true;
        }
        std::string setupAuthenticationPage(){
             /*std::string lURL = "xdg-open ";
             lURL.append("\"https://api.trakt.tv/oauth/authorize?response_type=code&client_id=3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07&redirect_uri=urn:ietf:wg:oauth:2.0:oob\"");
             system(lURL.c_str());*/
            JsonBuilder lBuilder;
            lBuilder.setStringValue(lBuilder.newEntity("client_id"), CLIENT_ID);
            FixMyTrakt_httpstraktmanager lPageManger;
            long lStatusCode = 0;
            lPageManger.postPage("https://api.trakt.tv/oauth/device/code", lBuilder.getFormatted().c_str(), &lStatusCode);
            if(lStatusCode!=200){
                
            }
            return "";
        }

        void getToken(const char *pPin){
            JsonBuilder lBuilder;
            lBuilder.setStringValue(lBuilder.newEntity("code"), pPin);
            lBuilder.setStringValue(lBuilder.newEntity("client_id"), CLIENT_ID);
            lBuilder.setStringValue(lBuilder.newEntity("client_secret"), CLIENT_SECRET);
            gLogger.log(lBuilder.getFormatted());
        }
    private:
        Logger gLogger = Logger("FixMTraktManager");
        DBManager gDBManager;
        // void getAuthenticationPage(){
        //     FixMyTrakt_httpstraktmanager lPageManger;
        //     std::string lURL = "xdg-open ";
        //     lURL.append(lPageManger.getRedirectionURL("https://api.trakt.tv/oauth/authorize?response_type=code&client_id=3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07&redirect_uri=urn:ietf:wg:oauth:2.0:oob"));
        //     system(lURL.c_str());
        // }
};