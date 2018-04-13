#include "Settings.h"

using namespace Settings;

SettingsHolder::SettingsHolder(){
    options_map[Options::HELP_LONG] = &IS_HELP;
    options_map[Options::HELP_SHORT] = &IS_HELP;
    options_map[Options::RECURSIVE] = &IS_RECURSIVE;
    options_map[Options::REVERSED_ORDER] = &IS_REVERSED_ORDER;
    options_map[Options::SPECIAL_FILE_TYPE] = &IS_SPECIAL_FILE_TYPE;
    options_map[Options::DETAILED_FILE_INFORMATION] = &IS_DETAILED_FILE_INFORMATION;

    options_map[SortingOptions::DIRECTORIES_FIRST] = &IS_DIRECTORIES_FIRST;
    options_map[SortingOptions::SPECIAL_FILES] = &IS_SPECIAL_FILES;
}

bool SettingsHolder::set(std::string option_name){
    if(options_map.find(option_name) == options_map.end())
        return false;
    
    *(options_map[option_name]) = true;
    return true;
}