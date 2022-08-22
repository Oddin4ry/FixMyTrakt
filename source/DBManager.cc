#include <filesystem>
#include <string>
#include <stdlib.h>
#include <iostream>

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
};