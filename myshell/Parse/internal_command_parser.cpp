#include "Parse.h"

using namespace Parse;

InternalCommandParser::InternalCommandParser(
    std::stringstream &strm,
    std::multiset<std::string>  &options
):
    strm(strm),
    raw(RawParser(strm)),
    basic(BasicParser(strm)),
    options(options)
    {}

bool InternalCommandParser::parse(){
    MERRNO = SUCCESS;
    
    strm >> std::skipws;
    char current;
    strm >> current; strm.putback(current);

    if(current == RAW_DELIMITER)
        options.insert(raw.parse(RAW_DELIMITER));
    else if(current == WHITESPACE_DELIMITER)
        options.insert(raw.parse(WHITESPACE_DELIMITER));
    else
        options.insert(basic.parse());
    
    return MERRNO == SUCCESS;
}