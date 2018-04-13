#include "ExternalCommand.h"

std::string ExternalCommand::get_real_path(std::string relative_path){
    static char BUFFER[PATH_MAX];

    realpath(relative_path.c_str(), BUFFER);

    return std::string(BUFFER);
}