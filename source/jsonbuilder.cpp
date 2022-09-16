#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif

#include <jsoncpp/json/json.h>

class JsonBuilder {
    enum {
        VALUE_TYPE_NONE,
        VALUE_TYPE_NUMBER,
        VALUE_TYPE_STRING
    };
    public:
        void parseString(const char *pJson){
            Json::Reader lReader;
            if(!lReader.parse(pJson, gRoot)){
                gLogger.log("Parse error");
            }
        }

        JsonBuilder removeParents(int pHowMany, JsonBuilder *pNewJson){
            for(int bCount=1;bCount<=pHowMany;bCount++){
                removeParent();
            }
        }

        std::string getFormatted(){
            std::stringstream lSS;
            Json::StyledStreamWriter lWriter;
            lWriter.write(lSS, gRoot);
            return lSS.str();
        }

        Json::Value newValue(const char *pName, const char *pValue){
            Json::Value lValue;
            lValue[pName] = pValue;
            gRoot.append(lValue);
            return lValue;
        }

        std::string getStringValue(const char *pName){
            Json::Value lValue = gRoot[pName];
            if(lValue!=0){
                return lValue.asString();
            }
            return "";
        }
        bool isEmpty(){
            return gRoot.empty();
        }

        int getSize(){
            return gRoot.size();
        }

        long long getIntegerValue(const char *pName){
            Json::Value lValue = gRoot[pName];
            if(lValue!=0){
                return lValue.asLargestInt();
            }
            return 0;
        }

    private:
        Json::Value gRoot;
        Logger gLogger = Logger("jsonbuilder");
};