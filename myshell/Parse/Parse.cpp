#include <boost/algorithm/string.hpp>
#include <vector>

#include "Parse.h"
#include "../Environ/Environ.h"

using namespace myshell;
using std::stringstream;
using std::string;
using std::noskipws;
using std::skipws;
using std::multiset;
using std::vector;

const char
    myshell::RAW_DELIMITER = '\'',
    myshell::WHITESPACE_DELIMITER = '\"';

const char
    myshell::ESCAPE_CHARACTER_INDICATOR = '\\',
    myshell::COMMENT_INDICATOR = '#',
    myshell::PIPE_INDICATOR = '|';

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
    char current; strm >> current;
    if(current == COMMENT_INDICATOR)
    {
        while(strm >> current);
        return true;
    }

    strm.putback(current);

    string currentOption;

    if(current == RAW_DELIMITER)
        currentOption = raw.parse(RAW_DELIMITER);
    else if(current == WHITESPACE_DELIMITER)
        currentOption = raw.parse(WHITESPACE_DELIMITER);
    else
    {
        currentOption = basic.parse();
        if(currentOption.front() == COMMENT_INDICATOR)
        {
            while(strm >> current);
            return true;
        }
    }
    
    if(current != RAW_DELIMITER)
        replaceEnvironmentVariables(currentOption);

    options.insert(currentOption);

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

void myshell::replaceEnvironmentVariables(string &target)
{
    target.push_back(' ');

    for(char ** envp = environ; *envp; ++envp)
    {
        string varAndValue{*envp};
        vector<string> tmp;

        boost::algorithm::split(tmp, varAndValue, boost::is_any_of(string(1, EQUALITY_SIGN)));

        string
            varName{tmp.front()},
            varValue{tmp.back()};
        
        boost::replace_all(target, "$" + varName + " ", varValue + " ");
    }

    if(target.back() == ' ')
        target.pop_back();
}