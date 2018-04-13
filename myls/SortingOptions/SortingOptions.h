#ifndef SORTING_OPTIONS_H_
#define SORTING_OPTIONS_H_

#include <string>

namespace SortingOptions{
    extern const std::string
        UNSORTED, // U
        SIZE, // S
        LAST_MODIFICATION_TIME, // t
        EXTENSION, // X
        NAME; // N
    
    extern const std::string
        DIRECTORIES_FIRST, // D
        SPECIAL_FILES; // s
}

#endif