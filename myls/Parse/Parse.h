#ifndef PARSE_H_
#define PARSE_H_

#include <string>

#include "../File/File.h"
#include "../Settings/Settings.h"

namespace myls{
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
            SettingsHolder &settingsHolder,
            FilesSorter &filesSorter
        );
        void parse();
    private:
        BasicParser basicParser;
        SettingsHolder &settingsHolder;
        FilesSorter &filesSorter;
    };
}

#endif