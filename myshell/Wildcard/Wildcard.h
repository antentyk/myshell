#ifndef WILDCARD_H_
#define WILDCARD_H_

#include <string>
#include <regex>
#include <set>

namespace myshell{
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