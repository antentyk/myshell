#ifndef TOKENS_H_
#define TOKENS_H_

#include <string>
#include <set>

namespace mymv{
    extern const std::string
        kHelpShort,
        kHelpLong,
        kForced;
    
    extern const std::string
        kYes,
        kNo,
        kAll,
        kCancel;
    
    extern const std::set<std::string>
        kReplacementAnswers;
}

#endif