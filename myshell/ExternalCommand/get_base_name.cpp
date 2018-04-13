#include "ExternalCommand.h"

std::string ExternalCommand::get_base_name(
    std::string path
)
{
    char* temp = new char[path.size() + 1];
    temp[path.size()] = '\0';
    strcpy(temp, path.c_str());

    char* result = basename(temp);

    std::string answer(result);

    delete [] temp;

    return answer;
}