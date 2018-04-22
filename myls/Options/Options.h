#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

namespace myls{
    extern const std::string
        HELP_SHORT, // -h
        HELP_LONG, // --help
        SORT, // --sort
        RECURSIVE, // -R
        REVERSED_ORDER, // -r
        SPECIAL_FILE_TYPE, // -F
        DETAILED_FILE_INFORMATION; // -l
}

#endif