#include <sstream>
#include <iostream>
#include <string.h>

#include "Wildcard.h"

using namespace myshell;
using std::set;
using std::string;
using std::stringstream;
using std::regex;
using std::regex_match;

namespace
{
    set<char> initWildcardIndicators(){
        set<char> result;

        result.insert('[');
        result.insert(']');
        result.insert('*');
        result.insert('?');

        return result;
    }
}

const set<char>
    myshell::WILDCARD_INDICATORS = initWildcardIndicators();

WildcardTransformer::WildcardTransformer(string &wildcard){
    stringstream regular;
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

    base_regex = regex(regular.str());
}

bool WildcardTransformer::match(string &candidate){
    return regex_match(candidate, base_match, base_regex);
}

bool myshell::is_wildcard(
    string &argument
){
    char* argument_for_basename = new char[argument.size() + 1];
    argument_for_basename[argument.size()] = 0;
    strcpy(argument_for_basename, argument.c_str());

    char* base_name = basename(argument_for_basename);

    bool result = false;

    for(size_t i = 0; i < strlen(base_name); i++)
        if
        (
            WILDCARD_INDICATORS.find(base_name[i]) !=
            WILDCARD_INDICATORS.end()
        )
        {
            result = true;
            break;
        }

    delete [] argument_for_basename;
    
    return result;
}