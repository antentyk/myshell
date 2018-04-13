#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <vector>
#include <map>

#include "../Options/Options.h"
#include "../SortingOptions/SortingOptions.h"

namespace Settings{
    class SettingsHolder{
    public:
        SettingsHolder();
        
        bool
            IS_HELP = false,
            IS_INVALID_OPTION = false,
            IS_RECURSIVE = false,
            IS_REVERSED_ORDER = false,
            IS_DETAILED_FILE_INFORMATION = false,
            IS_SPECIAL_FILE_TYPE = false;
        
        bool
            IS_DIRECTORIES_FIRST = false,
            IS_SPECIAL_FILES = false;

        std::map<std::string, bool*> options_map;
        std::vector<std::string> file_names;

        bool set(std::string option_name);
    };
}

#endif