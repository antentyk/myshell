#include "Parse.h"

using namespace Parse;

OptionsParser::OptionsParser(
    char **argv,
    Settings::SettingsHolder &settingsHolder,
    File::FilesSorter &filesSorter
):
    basicParser(BasicParser{argv}),
    settingsHolder(settingsHolder),
    filesSorter(filesSorter)
    {}

void OptionsParser::parse(){
    bool was_file_name_indicator = false;

    std::string current_option;

    while(basicParser.parse(&current_option)){
        if
        (
            current_option[0] == QUOTES &&
            current_option[current_option.size() - 1] == QUOTES
        )
        {
            if(current_option.size() <= 2)
                continue;
            current_option = current_option.substr(1,current_option.size() - 2);
            settingsHolder.file_names.push_back(current_option);
            continue;
        }
        
        if
        (
            was_file_name_indicator ||
            current_option[0] != OPTION_INDICATOR
        )
        {
            settingsHolder.file_names.push_back(current_option);
            continue;
        }

        if(current_option == FILE_NAME_INDICATOR){
            was_file_name_indicator = true;
            continue;
        }
        
        if
        (
            current_option.size() > Options::SORT.size() &&
            current_option.substr(0, Options::SORT.size() + 1) == Options::SORT + VALUE_DELIMITER
        )
            for(size_t i = Options::SORT.size() + 1; i < current_option.size(); i++){
                std::string sub_option = current_option.substr(i, 1);
                if
                (
                    !settingsHolder.set(sub_option) &&
                    !filesSorter.set(sub_option)
                )
                    settingsHolder.IS_INVALID_OPTION = true;
            }
        else if(!settingsHolder.set(current_option))
            settingsHolder.IS_INVALID_OPTION = true;
    }

    if(settingsHolder.file_names.size() == 0)
        settingsHolder.file_names.push_back("./");
}