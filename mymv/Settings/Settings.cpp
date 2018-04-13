#include "Settings.h"

using namespace mymv;
using std::string;

Settings::Settings():
isHelp_(false),
isForced_(false),
settingsMap_(
    {
        {kHelpLong, &isHelp_},
        {kHelpShort, &isHelp_},
        {kForced, &isForced_}
    }
)
{}

bool Settings::setArgument(string argument){
    auto it = settingsMap_.find(argument);

    if(it == settingsMap_.end())return false;
    
    return *it->second = true;
}