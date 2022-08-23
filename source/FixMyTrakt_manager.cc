#include "DBManager.cc"
#include "FixMyTrakt_httpstraktmanager.cc"
#include <string>

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER

#endif
class FixMyTrakt_manager{
    public:
        bool initialise(){
            openAuthenticationPage();
            return true;
        }
    private:
        Logger gLogger = Logger("FixMTraktManager");
        DBManager gDBManager;
        void openAuthenticationPage(){
             std::string lURL = "xdg-open ";
             lURL.append("\"https://api.trakt.tv/oauth/authorize?response_type=code&client_id=3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07&redirect_uri=urn:ietf:wg:oauth:2.0:oob\"");
             system(lURL.c_str());

        }
        // void getAuthenticationPage(){
        //     FixMyTrakt_httpstraktmanager lPageManger;
        //     std::string lURL = "xdg-open ";
        //     lURL.append(lPageManger.getRedirectionURL("https://api.trakt.tv/oauth/authorize?response_type=code&client_id=3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07&redirect_uri=urn:ietf:wg:oauth:2.0:oob"));
        //     system(lURL.c_str());
        // }
};