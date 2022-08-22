#include <string>
#include <curl/curl.h>

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif

#include <sstream>
class FixMyTrakt_httpstraktmanager{
    public:
        std::string getPage(const char *pURL){
            CURL* lCurl;
            std::stringstream lSS;
            lCurl= curl_easy_init();

            curl_easy_setopt(lCurl, CURLOPT_URL, pURL);
            curl_easy_setopt(lCurl, CURLOPT_WRITEDATA, &lSS);

            curl_easy_perform(lCurl);

            curl_easy_cleanup(lCurl);
            return lSS.str();
        }
        FixMyTrakt_httpstraktmanager(){
            curl_global_init(CURL_GLOBAL_ALL);
        }
        ~FixMyTrakt_httpstraktmanager(){
            curl_global_cleanup();
        }
    private:
        Logger gLogger = Logger("trakthttps");

};