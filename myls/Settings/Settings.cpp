#include "../Options/Options.h"
#include "../SortingOptions/SortingOptions.h"

#include "Settings.h"

using namespace myls;
using std::string;

SettingsHolder::SettingsHolder(){
    options_map[HELP_LONG] = &IS_HELP;
    options_map[HELP_SHORT] = &IS_HELP;
    options_map[RECURSIVE] = &IS_RECURSIVE;
    options_map[REVERSED_ORDER] = &IS_REVERSED_ORDER;
    options_map[SPECIAL_FILE_TYPE] = &IS_SPECIAL_FILE_TYPE;
    options_map[DETAILED_FILE_INFORMATION] = &IS_DETAILED_FILE_INFORMATION;

    options_map[DIRECTORIES_FIRST] = &IS_DIRECTORIES_FIRST;
    options_map[SPECIAL_FILES] = &IS_SPECIAL_FILES;
}

bool SettingsHolder::set(string option_name){
    if(options_map.find(option_name) == options_map.end())
        return false;
    
    *(options_map[option_name]) = true;
    return true;
}