#include "ExternalCommand.h"

bool ExternalCommand::is_directory(std::string file_name){
    struct stat path_stat;
    if(stat(file_name.c_str(), &path_stat) != 0)
        return false;
    return S_ISDIR(path_stat.st_mode);
}