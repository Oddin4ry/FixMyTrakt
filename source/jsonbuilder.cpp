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

        bool isValueValue(Json::Value pValue, const char *pCompare){
            if(pValue.isString()){
                return pValue.asString() == pCompare;
            }else if(pValue.isNumeric()){
                std::stringstream lSS;
                lSS << pValue.asLargestUInt();
                return lSS.str() == pCompare;
            }
            return false;
        }

        const char *displayValueValue(Json::Value pValue){
            if(pValue.isString()){
                return pValue.asCString();
            }else if(pValue.isNumeric()){
                std::stringstream lSS;
                lSS << pValue.asLargestUInt();
            }else if(pValue.isArray()){
                std::stringstream lSS;
                lSS << "Array of " << pValue.size();
            }else if(pValue.isObject()){
                return "Object";
            }
            return "Not sure";
        }

        std::string getFormatted(){
            std::stringstream lSS;
            Json::StyledStreamWriter lWriter;
            lWriter.write(lSS, gRoot);
            return lSS.str();
        }

        void addJsonVector(const char *pName, std::vector<Json::Value> pVector){
            Json::Value lArray;
            //gRoot.resize(pVector.size());
            for(Json::Value lItem : pVector){
                lArray.append(lItem);
            }
            //lArray.setComment(pName);
            Json::Value lNew;
            lNew[pName] = lArray;
            gRoot = lNew;
            //lNewParent << lArray;
     //       gRoot.append(lArray);
        }

        std::vector<Json::Value> getAllChilden(const std::string pKey){
            std::vector<Json::Value> lResults;
            for (int i = 0; i < gRoot.size(); i++){
                lResults.push_back(gRoot[i][pKey]);
                //for(auto const& id :gRoot[i].getMemberNames()){
                  //  if()
                //}
                /*if(gRoot[i].asString()==pKey){
                    gLogger.log("Found ", gRoot[i].asString());
                    lResults.push_back(gRoot[i]);
                }*/
            }
            return lResults;
          /*assert(json.isArray());
    std::vector<Json::Value> results;
    for (size_t i = 0; i != json.size(); i++)
        if (json[i][key].asString() == value)
            results.push_back(json[i]);
    return results;*/  
        }

        Json::Value newValue(const char *pName, const char *pValue){
            Json::Value lValue;
            lValue[pName] = pValue;
            gRoot.append(lValue);
            return lValue;
        }

        void setRoot(const char *pName){
            gRoot = pName;
        }

        Json::Value newValue(Json::Value pParent, const char *pName, const char *pValue){
            Json::Value lValue;
            lValue[pName] = pValue;
            pParent.append(lValue);
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