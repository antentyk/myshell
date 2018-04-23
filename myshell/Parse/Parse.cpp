#include "Parse.h"

using namespace myshell;
using std::stringstream;
using std::string;
using std::noskipws;
using std::skipws;
using std::multiset;

const char
    myshell::RAW_DELIMITER = '\'',
    myshell::WHITESPACE_DELIMITER = '\"';

const char
    myshell::ESCAPE_CHARACTER_INDICATOR = '\\';

RawParser::RawParser(stringstream &strm):
    strm(strm){}

string RawParser::parse(char delimiter){
    strm >> noskipws;

    stringstream result;

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

BasicParser::BasicParser(stringstream &strm):
    strm(strm){}

string BasicParser::parse(){
    strm >> skipws;

    string result;
    strm >> result;

    return result;
}

InternalCommandParser::InternalCommandParser(
    stringstream &strm,
    multiset<string>  &options
):
    strm(strm),
    raw(RawParser(strm)),
    basic(BasicParser(strm)),
    options(options)
    {}

bool InternalCommandParser::parse(){
    MERRNO = SUCCESS;
    
    strm >> skipws;
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

bool myshell::test_stream(stringstream &strm){
    strm >> skipws;

    char current; strm >> current;

    if(strm.fail())
        return false;
        
    strm.putback(current);
    return true;
}