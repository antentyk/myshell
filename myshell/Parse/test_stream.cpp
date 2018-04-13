#include "Parse.h"

bool Parse::test_stream(std::stringstream &strm){
    strm >> std::skipws;

    char current; strm >> current;

    if(strm.fail())
        return false;
        
    strm.putback(current);
    return true;
}