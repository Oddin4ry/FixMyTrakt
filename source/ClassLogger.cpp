#include <string>
#include <filesystem>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;

class Logger {
    public:
        Logger(std::string pLogName){
            db_file = fs::path(getenv("HOME"));
            fs::current_path(db_file);
            db_file.append("/FixMyTrakt/");
            fs::create_directory(db_file);
            db_file.append("Logs/");
            fs::create_directory(db_file);
            db_file.append(pLogName);
            time_t now = time(0);
            struct tm tstruct;
            char buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
            db_file.append(buf);
            db_file.append(".log");
        }

        void log(std::string pWhatToLog){
            log(pWhatToLog.c_str());
        }

        void log(const char *pWhatToLog, int pNumberToLog){
            std::stringstream lSS;
            lSS << pWhatToLog << ": " << pNumberToLog;
            log(lSS.str());
        }

        void log(const char *pWhatToLog, long pNumberToLog){
            std::stringstream lSS;
            lSS << pWhatToLog << ": " << pNumberToLog;
            log(lSS.str());
        }


        void log(const char *pWhatToLog, bool pBoleanToLog){
            std::stringstream lSS;
            lSS << pWhatToLog << ": ";
            if(pBoleanToLog){
                lSS << "true";
             }else{
                lSS << "false";
             }
            log(lSS.str());
        }

        void log(const char *pWhatToLog, char pCharacterToLog){
            std::stringstream lSS;
            lSS << pWhatToLog << ": " << pCharacterToLog;
            log(lSS.str());
        }

        void log(const char *pWhatToLog, const char *pValue){
            std::stringstream lSS;
            lSS << pWhatToLog << ": " << pValue;
            log(lSS.str());
        }

        void log(const char *pWhatToLog, std::string pValue){
            std::stringstream lSS;
            lSS << pWhatToLog << ": " << pValue;
            log(lSS.str());
        }

        void log(const char *pWhatToLog){
            std::ofstream ofs(db_file.c_str(), std::ios_base::out | std::ios_base::app);
            time_t now = time(0);
            struct tm tstruct;
            char buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%X: ", &tstruct);
            ofs << buf << pWhatToLog << '\n';
            std::cout << buf << pWhatToLog << std::endl;
            ofs.close();
        }

    private:
        std::string db_file = "";    
};