#include "Parse.h"

using namespace Parse;

RawParser::RawParser(std::stringstream &strm):
    strm(strm){}

std::string RawParser::parse(char delimiter){
    strm >> std::noskipws;

    std::stringstream result;

    char current; strm >> current;
    while(strm >> current){
        if(current == delimiter)break;

        if(current == ESCAPE_CHARACTER_INDICATOR){
            strm >> current;
            if(strm.fail())break;

            result << current;
            current = 0;
            continue;
        }

        result << current;
    }

    if(current != delimiter){
        mfail
        (
            "Error while parsing optoins: no enclosing token",
            PARSE_TOKENS_FAIL
        );
        return "";
    }

    return result.str();
}