#include "Parse.h"

using namespace Parse;

BasicParser::BasicParser(std::stringstream &strm):
    strm(strm){}

std::string BasicParser::parse(){
    strm >> std::skipws;

    std::string result;
    strm >> result;

    return result;
}