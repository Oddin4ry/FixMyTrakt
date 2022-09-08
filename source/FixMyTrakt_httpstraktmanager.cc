#include <string>
#include <curl/curl.h>
#include <algorithm>
#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif

#include <sstream>
class FixMyTrakt_httpstraktmanager{
    public:
        void setDefaultHeaderDetails(const char *pClientId, const char *pApiVersion, const char *pAccessToken){
            gClientId = pClientId;
            gApiVersion = pApiVersion;
            gAccessTokken = pAccessToken;
        }

        static size_t WriteCallBack(char *pContents, size_t pSize, size_t pNMemb, void *pUserP){
            return static_cast<FixMyTrakt_httpstraktmanager*>(pUserP)->HandleDataIn(pContents, pSize, pNMemb);
        }

        size_t HandleDataIn(char *pData, size_t pSize, size_t pNMemb){
            gInBuffer.append(pData);
            return pSize * pNMemb;
        }


        std::string getRedirectionURL(const char *pURL){
            std::string lPage = getPageWithHeader(pURL);
            return getTag("location: ", "\r", lPage.c_str());
        }

        std::string getPage(const char *pURL, long *pStatusCode){
            CURL* lCurl;
            std::stringstream lSS;
            lCurl= curl_easy_init();
            FixMyTrakt_httpstraktmanager lOtherCopy;
            curl_easy_setopt(lCurl, CURLOPT_URL, pURL);
            curl_easy_setopt(lCurl, CURLOPT_WRITEDATA, &lOtherCopy);
            curl_easy_setopt(lCurl, CURLOPT_WRITEFUNCTION, &FixMyTrakt_httpstraktmanager::WriteCallBack);
            curl_easy_setopt(lCurl, CURLOPT_HEADER, 0);
            curl_easy_setopt(lCurl, CURLOPT_VERBOSE, 0);
    
            struct curl_slist *hs=NULL;
            if(gIsJSON){
                hs = curl_slist_append(hs, "Content-Type: application/json");
            }

            if(gAccessTokken!=""){
                std::stringstream bSS;
                bSS << "authorization: bearer " << gAccessTokken;
                hs = curl_slist_append(hs, bSS.str().c_str());
            }
            if(gClientId!=""){
                std::stringstream bSS;
                bSS << "trakt-api-key: " << gClientId;
                hs = curl_slist_append(hs, bSS.str().c_str());
            }
            if(gApiVersion!=""){
                std::stringstream bSS;
                bSS << "trakt-api-version " << gApiVersion;
                hs = curl_slist_append(hs, bSS.str().c_str());
            } 
            curl_easy_setopt(lCurl, CURLOPT_HTTPHEADER, hs);

            curl_easy_perform(lCurl);

            curl_easy_getinfo(lCurl, CURLINFO_RESPONSE_CODE, pStatusCode);

            curl_easy_cleanup(lCurl);
            gLogger.log("GET PAGE", pURL);
            gLogger.log(lOtherCopy.gInBuffer);
            return lOtherCopy.gInBuffer;
        }

        std::string postPage(const char *pURL, const char *pPostBody, long *pStatusCode){
            CURL* lCurl;
            std::stringstream lSS;
            lCurl= curl_easy_init();
            FixMyTrakt_httpstraktmanager lOtherCopy;
            curl_easy_setopt(lCurl, CURLOPT_URL, pURL);
            curl_easy_setopt(lCurl, CURLOPT_WRITEDATA, &lOtherCopy);
            curl_easy_setopt(lCurl, CURLOPT_WRITEFUNCTION, &FixMyTrakt_httpstraktmanager::WriteCallBack);
            curl_easy_setopt(lCurl, CURLOPT_HEADER, 0);
            curl_easy_setopt(lCurl, CURLOPT_VERBOSE, 0);
            if(gIsJSON){
                struct curl_slist *hs=NULL;
                hs = curl_slist_append(hs, "Content-Type: application/json");
                hs = curl_slist_append(hs, "Accept: application/json");
                curl_easy_setopt(lCurl, CURLOPT_HTTPHEADER, hs);
            }
            curl_easy_setopt(lCurl, CURLOPT_POST, 1);
            curl_easy_setopt(lCurl, CURLOPT_POSTFIELDS, pPostBody);
            
            curl_easy_perform(lCurl);
            
            curl_easy_getinfo(lCurl, CURLINFO_RESPONSE_CODE, pStatusCode);

            curl_easy_cleanup(lCurl);
            gLogger.log("POST PAGE");
            gLogger.log(pURL);
            gLogger.log(lOtherCopy.gInBuffer);
            return lOtherCopy.gInBuffer;
        }


        std::string getPageWithHeader(const char *pURL){
            CURL* lCurl;
            std::stringstream lSS;
            lCurl= curl_easy_init();
            FixMyTrakt_httpstraktmanager lOtherCopy;
            curl_easy_setopt(lCurl, CURLOPT_URL, pURL);
            curl_easy_setopt(lCurl, CURLOPT_WRITEDATA, &lOtherCopy);
            curl_easy_setopt(lCurl, CURLOPT_WRITEFUNCTION, &FixMyTrakt_httpstraktmanager::WriteCallBack);
            curl_easy_setopt(lCurl, CURLOPT_HEADER, 1);
            curl_easy_setopt(lCurl, CURLOPT_VERBOSE, 0);

            curl_easy_perform(lCurl);

            curl_easy_cleanup(lCurl);
            gLogger.log("GET PAGE WITH HEADER");
            gLogger.log(pURL);
            gLogger.log(lOtherCopy.gInBuffer);
            return lOtherCopy.gInBuffer;
        }
        FixMyTrakt_httpstraktmanager(){
            curl_global_init(CURL_GLOBAL_ALL);
        }
        ~FixMyTrakt_httpstraktmanager(){
            curl_global_cleanup();
        }

        

        bool gIsJSON = true;

    private:
        Logger gLogger = Logger("trakthttps");
        std::string gInBuffer = "";
        std::string getTag(const char *pStartTag, const char *pEndTag, const char *pContents){
            std::string lContents = pContents;
            std::string lStartTag = pStartTag;
            std::string lEndTag = pEndTag;
            std::transform(lContents.begin(), lContents.end(), lContents.begin(), [](unsigned char c){ return toupper(c); });
            std::transform(lStartTag.begin(), lStartTag.end(), lStartTag.begin(), [](unsigned char c){ return toupper(c); });
            std::transform(lEndTag.begin(), lEndTag.end(), lEndTag.begin(), [](unsigned char c){ return toupper(c); });
            size_t lStart = lContents.find(lStartTag);
            if(lStart==std::string::npos){
                return "";
            }
            lStart = lStart + lStartTag.length();
            size_t lEnd = lContents.find(lEndTag, lStart);
            if(lEnd==std::string::npos){
                return "";
            }
            lContents = pContents;
            return lContents.substr(lStart, lEnd - lStart);
        }

        std::string gClientId = "";
        std::string gApiVersion = "";
        std::string gAccessTokken = "";
};