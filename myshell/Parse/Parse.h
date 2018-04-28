#ifndef PARSE_H_
#define PARSE_H_

#include <string>
#include <sstream>
#include <set>

#include "../Common/Common.h"

namespace myshell{
    extern const char
        RAW_DELIMITER,
        WHITESPACE_DELIMITER;
    extern const char
        ESCAPE_CHARACTER_INDICATOR,
        COMMENT_INDICATOR;
    
    extern const int
        PARSE_TOKENS_FAIL;

    class BasicParser{
    public:
        BasicParser(std::stringstream &strm);
        std::string parse();
    private:
        std::stringstream &strm; 
    };

    class RawParser{
    public:
        RawParser(std::stringstream &strm);
        std::string parse(char delimiter);
    private:
        std::stringstream &strm;
    };

    class InternalCommandParser{
    public:
        InternalCommandParser(
            std::stringstream &strm,
            std::multiset<std::string>  &options
        );
        bool parse();
    private:
        std::stringstream &strm;
        RawParser raw;
        BasicParser basic;
        std::multiset<std::string> &options;
    };

    bool test_stream(std::stringstream &strm);

    void replaceEnvironmentVariables(std::string &target);
}

#endif