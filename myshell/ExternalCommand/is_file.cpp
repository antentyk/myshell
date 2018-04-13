#include "ExternalCommand.h"

bool ExternalCommand::is_file(std::string file_name){
    struct stat path_stat;
    if(stat(file_name.c_str(), &path_stat) != 0)
        return false;
    return S_ISREG(path_stat.st_mode);
}