#include "Settings.h"

using namespace mymkdir;
using std::string;

Settings::Settings():
isHelp_(false),
isParents_(false),
settingsMap_(
    {
        {kHelpLong, &isHelp_},
        {kHelpShort, &isHelp_},
        {kParents, &isParents_}
    }
)
{}

bool Settings::setArgument(string argument){
    auto it = settingsMap_.find(argument);

    if(it == settingsMap_.end())return false;
    
    return *it->second = true;
}