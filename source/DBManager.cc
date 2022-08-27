#include <filesystem>
#include <string>
#include <stdlib.h>

namespace fs = std::filesystem;

class DBManager{
    public:
        
        std::string db_path = "";
        DBManager(){
            db_path = fs::path(getenv("HOME"));
            fs::current_path(db_path);
            db_path.append("/FixMyTrakt/");
            fs::create_directory(db_path);
            db_path.append("DB/");
            fs::create_directory(db_path);
        }
    public:
        void beginUpdate(std::string pDBName){
            
        }
        void endUpdate(){
            //static_assert("test");
        }
};