#include "ExternalCommand.h"

bool ExternalCommand::is_executable(std::string file_name){
    struct stat path_stat;
    if(stat(file_name.c_str(), &path_stat) != 0)
        return false;
    return path_stat.st_mode & S_IXUSR;
}