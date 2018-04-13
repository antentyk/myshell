#include "Wildcard.h"

bool Wildcard::is_wildcard(
    std::string &argument
){
    char* argument_for_basename = new char[argument.size() + 1];
    argument_for_basename[argument.size()] = 0;
    strcpy(argument_for_basename, argument.c_str());

    char* base_name = basename(argument_for_basename);

    bool result = false;

    for(size_t i = 0; i < strlen(base_name); i++)
        if
        (
            Wildcard::WILDCARD_INDICATORS.find(base_name[i]) !=
            Wildcard::WILDCARD_INDICATORS.end()
        )
        {
            result = true;
            break;
        }

    delete [] argument_for_basename;
    
    return result;
}