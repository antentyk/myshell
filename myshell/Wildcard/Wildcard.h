#ifndef WILDCARD_H_
#define WILDCARD_H_

#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <set>

namespace Wildcard{
    extern const
        std::set<char> WILDCARD_INDICATORS;

    bool is_wildcard(std::string &argument);

    class WildcardTransformer{
    public:
        WildcardTransformer(std::string &wildcard);
        bool match(std::string &candidate);
    private:
        std::regex base_regex;
        std::smatch base_match;
    };
}

#endif