#include "Parse.h"

using namespace Parse;

BasicParser::BasicParser(char **argv):
    argv(argv){}

bool BasicParser::parse(std::string *target){
    if(*(argv) == NULL)return false;

    *target = std::string(*argv);
    ++argv;

    return true;
}