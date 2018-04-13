#include "ExternalCommand.h"

std::string ExternalCommand::get_dir_name(
    std::string path
)
{
    char* temp = new char[path.size() + 1];
    temp[path.size()] = '\0';
    strcpy(temp, path.c_str());

    char* result = dirname(temp);

    std::string a(result);

    delete [] temp;

    return a;
}