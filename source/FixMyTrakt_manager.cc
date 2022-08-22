#include "DBManager.cc"
#include "FixMyTrakt_httpstraktmanager.cc"
#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif
class FixMyTrakt_manager{
    public:
        void initialise(){
            authenticate();
        }
    private:
        Logger gLogger = Logger("FixMTraktManager");
        DBManager gDBManager;
        void authenticate(){
            FixMyTrakt_httpstraktmanager lPageManger;
            gLogger.log(lPageManger.getPage("https://api.trakt.tv/oauth/authorize?response_type=code&client_id=3cd645fbc46049abcb1820ec3e02314ac9e26ec3b7b8f0fa05c4a97a8a236c07&redirect_uri=urn:ietf:wg:oauth:2.0:oob"));
        }
};