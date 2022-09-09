#include <filesystem>
#include <string>
#include <stdlib.h>

#ifndef JSON_BUILDER
#include "jsonbuilder.cpp"
#define JSON_BUILDER
#endif

namespace fs = std::filesystem;

class DBManager{
    public:
        const char *DB_TOKENS = "TOKENS";
        std::string db_path = "";
        DBManager(){
            db_path = fs::path(getenv("HOME"));
            fs::current_path(db_path);
            db_path.append("/FixMyTrakt/");
            fs::create_directory(db_path);
            db_path.append("DB/");
            fs::create_directory(db_path);
        }

        void doRead(std::string pDBName){
            gFileName = db_path;
            gFileName.append(pDBName);
            gFileName.append(".txt");
            if(gBuilder!=0){
                delete gBuilder;
            }
            gBuilder = new JsonBuilder();
            std::ifstream lFile;
            lFile.open(gFileName, std::ifstream::in);
            if(lFile.is_open()){
                std::stringstream lSS;
                while(!lFile.eof()){
                    std::string bSS;

                    lFile >> bSS;
                    lSS << bSS;
                }
                lFile.close();
                gBuilder->parseString(lSS.str().c_str());
            }
        }

        void beginUpdate(std::string pDBName){
            gFileName = db_path;
            gFileName.append(pDBName);
            gFileName.append(".txt");
            if(gBuilder!=0){
                delete gBuilder;
            }
            gBuilder = new JsonBuilder();
            doFileExistsRead();
        }

        std::string getStringValue(const char *pField){
            return gBuilder->getStringValue(pField);
        }

        void setStringValue(const char *pFieldName, const char *pValue){
            gBuilder->newValue(pFieldName, pValue);
        }

        ~DBManager(){
            delete gBuilder;
        }

        void endUpdate(){
            std::ofstream lFile(gFileName);
            lFile << gBuilder->getFormatted();
            lFile.close();
            delete gBuilder;
            gBuilder = 0;
        }


    private:
        void doFileExistsRead(){
            std::ifstream lFile;
            lFile.open(gFileName, std::ifstream::in);
            if(lFile.is_open()){
                std::stringstream lSS;
                while(!lFile.eof()){
                    std::string bSS;

                    lFile >> bSS;
                    lSS << bSS;
                }
                lFile.close();
                gBuilder->parseString(lSS.str().c_str());

                std::string lBackup = gFileName;
                lBackup.append(".old");
                std::ofstream bFile(lBackup);
                bFile << lSS.str();
                bFile.close();
            }


        }
        JsonBuilder *gBuilder = 0;
        std::string gFileName = "";
        Logger gLogger = Logger("db manager");

};