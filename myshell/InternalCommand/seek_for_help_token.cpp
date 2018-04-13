#include "InternalCommand.h"

bool InternalCommand::seek_for_help_token(std::multiset<std::string> &target){
    bool is_help = false;

    std::set<std::string>::iterator help_seeker;

    help_seeker = target.find(Tokens::HELP_SHORT);
    if(help_seeker != target.end()){
        is_help = true;
        target.erase(Tokens::HELP_SHORT);
    }

    help_seeker = target.find(Tokens::HELP_LONG);
    if(help_seeker != target.end()){
        is_help = true;
        target.erase(Tokens::HELP_LONG);
    }
    
    return is_help;
}
