#include "Wildcard.h"

using namespace Wildcard;

WildcardTransformer::WildcardTransformer(std::string &wildcard){
    std::stringstream regular;
    for(auto it: wildcard){
        if(it == '?')
            regular << '.';
        else if(it == '*')
            regular << ".*";
        else if(it == '.')
            regular << "\\.";
        else
            regular << it;
    }

    base_regex = std::regex(regular.str());
}

bool WildcardTransformer::match(std::string &candidate){
    return std::regex_match(candidate, base_match, base_regex);
}