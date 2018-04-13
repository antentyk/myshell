#ifndef PARSE_H_
#define PARSE_H_

#include <string>
#include <sstream>
#include <algorithm>

#include "../File/File.h"
#include "../Settings/Settings.h"
#include "../Options/Options.h"

namespace Parse{
    extern const char
        QUOTES, // "
        VALUE_DELIMITER, // =
        OPTION_INDICATOR; // -
    
    extern const std::string
        FILE_NAME_INDICATOR; // --

    class BasicParser{
    public:
        BasicParser(char **argv);
        bool parse(std::string *target);
    private:
        char **argv;
    };

    class OptionsParser{
    public:
        OptionsParser
        (
            char **argv,
            Settings::SettingsHolder &settingsHolder,
            File::FilesSorter &filesSorter
        );
        void parse();
    private:
        BasicParser basicParser;
        Settings::SettingsHolder &settingsHolder;
        File::FilesSorter &filesSorter;
    };
}

#endif