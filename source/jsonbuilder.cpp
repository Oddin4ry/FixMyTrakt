#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif

class JsonBuilder {
    enum {
        VALUE_TYPE_NONE,
        VALUE_TYPE_NUMBER,
        VALUE_TYPE_STRING
    };
    struct Entity{
        std::string name = "";
        std::string svalue = "";
        long long ivalue = 0;
        short type = 0;
        Entity *allChildren = 0;
        Entity *last = 0;
        Entity *previous = 0;
        Entity *next = 0;
    };
    public:
        void buildFromString(const char *pJson){
            std::stringstream lCollector;
            std::string lJson = pJson;
            bool lStarted = false;
            bool lStartedQuote = false;
            
            for(char lChar: lJson){
                if(!lStarted&&lChar=='{'){
                    lStarted = true;
                }else{
                    if(lChar='"'){
                        lStartedQuote = !lStartedQuote;
                    }
                }
            }
        }

        std::string getFormatted(){
            if(gAllEntities==0){
                return "";
            }

            std::stringstream lSS;
            lSS << "{" << std::endl;
            Entity *lEntity = gAllEntities;
            while(lEntity!=0){
                lSS << getFormatted(lEntity, 1);
                lEntity = lEntity->next;
            }
            lSS << "}";
            return lSS.str();
        }

        void setStringValue(Entity *pEntity, const char *pValue){
            pEntity->type = VALUE_TYPE_STRING;
            std::string lString = pValue;
            pEntity->svalue = convertString(lString);
        }

        void setIntegerValue(Entity *pEntity, long long pValue){
            pEntity->type = VALUE_TYPE_STRING;
            pEntity->ivalue = pValue;
        }

        Entity *newEntity(const char *pName){
            return newEntity(0, pName);
        }

        Entity *newEntity(Entity *pParent, const char *pName){
            Entity *lNew = new Entity;
            lNew->name = pName;
            if(pParent!=0){
                if(pParent->allChildren==0){
                    pParent->allChildren = lNew;
                }else{
                    pParent->last->next = lNew;
                    lNew->previous = pParent->last;
                }
                pParent->last = lNew;
            }else{
                if(gAllEntities==0){
                    gLogger.log("NEW");
                    gAllEntities = lNew;
                }else{
                    lNew->previous = gLast;
                    gLast->next = lNew;
                }

                gLast = lNew;
            }
            return lNew;
        }

        void deleteSubEntity(Entity *pParent){
            Entity *lEntity = pParent->allChildren;
            while(lEntity!=0){
                Entity *bNext = lEntity->next;
                deleteSubEntity(lEntity);
                delete lEntity;
                lEntity = bNext;
            }
        }

        ~JsonBuilder(){
            Entity *lEntity = gAllEntities;
            while(lEntity!=0){
                Entity *bNext = lEntity->next;
                deleteSubEntity(lEntity);
                delete lEntity;
                lEntity = bNext;
            }
        }

    private:
        Entity *gCurrentEntity = 0;
        Entity *gLast = 0;
        Entity *gAllEntities = 0;
        std::string getFormatted(Entity *pEntity, const short pTabs){
            std::stringstream lSS;
            if(pEntity->name!=""){
                lSS << std::string(pTabs, '\t') << "\"" << pEntity->name << "\"";
            }
            if(pEntity->type==VALUE_TYPE_NUMBER){
                lSS << ": " << pEntity->ivalue;
            }else if(pEntity->type==VALUE_TYPE_STRING){
                lSS << ": \"" << pEntity->svalue << "\"";
            }
            if(pEntity->next!=0){
                lSS << ",";
            }
            Entity *lEntity = pEntity->allChildren;
            while(lEntity!=0){
                lSS << ": {" << std::endl;
                lSS << getFormatted(lEntity, pTabs+1);
                lEntity = lEntity->next;
                lSS << std::string(pTabs, '\t') << "}" << std::endl;
            }
            lSS << std::endl;
            return lSS.str();
        }

        // https://docs.microsoft.com/en-us/sql/relational-databases/json/how-for-json-escapes-special-characters-and-control-characters-sql-server?view=sql-server-ver16
        std::string convertString(std::string pInput){
            std::stringstream lSS;
            for(char lChar: pInput){
                if(lChar=='\"') {
                    lSS << "\\\"";
                }else if(lChar=='\\'){
                    lSS << "\\\\";
                }else if(lChar=='/'){
                    lSS << "\\/";
                }else if(lChar==9){
                    lSS << "\\t";
                }else if(lChar==13){
                    lSS << "\\r";
                }else if(lChar==10){
                    lSS << "\\n";
                }else if(lChar<32){
                    lSS << "\\" << std::setfill('0') << std::setw(4) << std::hex << (int)lChar;
                }else{
                    lSS << lChar;
                }
            }
            return lSS.str();

        }
        Logger gLogger = Logger("jsonbuilder");
};