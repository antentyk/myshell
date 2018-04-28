#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "../Common/Common.h"
#include "../Parse/Parse.h"
#include "../Tokens/Tokens.h"
#include "../Environ/Environ.h"

#include "InternalCommand.h"

using namespace myshell;
using std::cout;
using std::endl;
using std::stringstream;
using std::string;
using std::multiset;
using std::set;
using std::ifstream;
using std::vector;

void myshell::display_help_tokens(){
    cout << "[";
    cout << HELP_SHORT;
    cout << "|";
    cout << HELP_LONG;
    cout << "]";
}

void myshell::display_help_merrno(){
    cout << MERRNO << " ";

    display_help_tokens();
    cout << endl;

    cout << "Displays end code of the last command executed";
    cout << std::endl;

    cout << "If no commands were executed, displays ";
    cout << SUCCESS << endl;
}

void myshell::display_help_script(){
    cout << MSCRIPT << " ";

    display_help_tokens();
    cout << "<path to script>" << endl;

    cout << "Runs script in current window" << endl;
}

void myshell::display_help_mpwd(){
    cout << MPWD << " ";

    display_help_tokens();
    cout << endl;

    cout << "Displays the current path" << endl;
}

void myshell::display_help_mcd(){
    cout << MCD;

    cout << " <path> ";

    display_help_tokens();
    cout << endl;

    cout << "Navigates to the provided directory" << endl;
}

void myshell::display_help_mexit(){
    cout << MEXIT;

    cout << " <exit code> ";

    display_help_tokens();
    cout << std::endl;

    cout << "Terminates myshell session with given exit code";
    cout << endl;

    cout << "If no code is procided, exits with ";
    cout << SUCCESS << endl;
}

void myshell::run_mcd(
    stringstream &strm
){

    multiset<string> options;

    InternalCommandParser parser(
        strm,
        options
    );

    ::MERRNO = SUCCESS;

    while(test_stream(strm))
        if(!parser.parse())
            return;
    
    if(seek_for_help_token(options)){
        ::MERRNO = SUCCESS;
        return display_help_mcd();
    }

    if(options.size() == 0)
        return mfail(
            "Too few options",
            MCD_FAIL
        );
    
    if(options.size() > 1)
        return mfail(
            "Too many options",
            MCD_FAIL
        );
    
    string new_path = *(options.begin());
    int success = chdir(new_path.c_str());

    if(success < 0)
        return mfail("Invalid path", MCD_FAIL);

    ::MERRNO = SUCCESS;
}

void myshell::run_merrno(
    stringstream &strm
){

    multiset<string> options;

    InternalCommandParser parser(
        strm,
        options
    );

    int previous_merrno = ::MERRNO;

    ::MERRNO = SUCCESS;

    while(test_stream(strm))
        if(!parser.parse())
            return;
    
    if(seek_for_help_token(options)){
        ::MERRNO = SUCCESS;
        return display_help_merrno();
    }
    
    if(options.size() > 0)
        return mfail("Too many arguments", MERRNO_FAIL);
    
    cout << previous_merrno << endl;
    ::MERRNO = SUCCESS;
}

void myshell::run_mexit(
    stringstream &strm
){
    multiset<string> options;

    InternalCommandParser parser(
        strm,
        options
    );

    ::MERRNO = SUCCESS;

    while(test_stream(strm))
        if(!parser.parse())
            return;
    
    if(seek_for_help_token(options)){
        ::MERRNO = SUCCESS;
        return display_help_mexit();
    }

    if(options.size() > 1)
        return mfail("Too many options", MEXIT_FAIL);

    int exit_code;
    if(options.size() == 0)
        exit_code = SUCCESS;
    else{
        stringstream tmp;
        tmp << *(options.begin());
        tmp >> exit_code;
        if(tmp.fail())
            return mfail(
                "Invalid exit code",
                MEXIT_FAIL
            );
    }
    
    ::MERRNO = SUCCESS;
    exit(exit_code);
}

void myshell::run_mpwd(
    stringstream &strm
){
    multiset<string> options;

    InternalCommandParser parser(
        strm,
        options
    );

    ::MERRNO = SUCCESS;

    while(test_stream(strm))
        if(!parser.parse())
            return;
    
    if(seek_for_help_token(options)){
        ::MERRNO = SUCCESS;
        return display_help_mpwd();
    }

    if(options.size() > 0)
        return mfail("Too many options", MPWD_FAIL);
    
    ::MERRNO = SUCCESS;
    
    static char BUFFER[PATH_MAX];
    char *result = getcwd(BUFFER, PATH_MAX);

    if(result == NULL)
        return mfail(
            "Error occured while getting current path",
            MPWD_FAIL
        );
    
    cout << result << endl;
}

void myshell::run_script(stringstream &strm)
{
    multiset<string> options;

    InternalCommandParser parser(
        strm,
        options
    );

    while(test_stream(strm))
        if(!parser.parse())
            return;
    
    if(seek_for_help_token(options))
    {
        ::MERRNO = SUCCESS;
        return display_help_script();
    }

    if(options.size() != 1)
        return mfail("Invalid number of arguments", SCRIPT_FAIL);

    string scriptName = *options.begin();

    struct stat buffer;
    if(stat(scriptName.c_str(), &buffer) == -1)
        return mfail("Cannot find script file", SCRIPT_FAIL);
    if(!S_ISREG(buffer.st_mode))
        return mfail("Script must be regular file", SCRIPT_FAIL);

    ifstream scriptFile;
    scriptFile.open(scriptName);

    if(!scriptFile.is_open())
        return mfail("Cannot open script file", SCRIPT_FAIL);
    
    ::MERRNO = SUCCESS;

    string line;
    while(getline(scriptFile, line))
        execute(line);
}

bool myshell::seek_for_help_token(multiset<string> &target){
    bool is_help = false;

    set<string>::iterator help_seeker;

    help_seeker = target.find(HELP_SHORT);
    if(help_seeker != target.end()){
        is_help = true;
        target.erase(HELP_SHORT);
    }

    help_seeker = target.find(HELP_LONG);
    if(help_seeker != target.end()){
        is_help = true;
        target.erase(HELP_LONG);
    }
    
    return is_help;
}

void myshell::run_mexport(stringstream &strm)
{
    ::MERRNO = SUCCESS;

    multiset<string> options;

    InternalCommandParser parser(strm, options);

    while(test_stream(strm))
        if(!parser.parse())
            return;
    
    if(options.size() > 1)
        return mfail("Invalid number of arguments", MEXPORT_FAIL);
    
    string content{*options.begin()};

    string varName;

    if(boost::contains(content, string(1, EQUALITY_SIGN)))
    {
        stringstream tmp{content};
        run_variable(tmp);

        varName = content.substr(0, content.find(EQUALITY_SIGN, 0));
    }
    else
        varName = content;
    
    exportVar(varName);
}

void myshell::run_variable(stringstream &strm)
{
    ::MERRNO = SUCCESS;

    string content{strm.str()};

    if(!boost::contains(content, string(1, EQUALITY_SIGN)))
        return mfail("Cannot find equality sign", ENVIRONMENT_FAIL);

    size_t delimiterIndex = content.find(EQUALITY_SIGN);

    string varName = content.substr(0, delimiterIndex);

    stringstream valueStrm{content.substr(delimiterIndex + 1)};
    multiset<string> valueOptions;
    
    InternalCommandParser parser(valueStrm, valueOptions);
    while(test_stream(valueStrm))
        if(!parser.parse())
            return mfail("Error while parsing rhs value", ENVIRONMENT_FAIL);

    if(valueOptions.size() != 1)
        return mfail("Invalid number of arguments for rhs", ENVIRONMENT_FAIL);

    setVar(varName, *valueOptions.begin());
}

void myshell::run_mecho(stringstream &strm)
{
    ::MERRNO = SUCCESS;

    multiset<string> options;

    InternalCommandParser parser(strm, options);

    while(test_stream(strm) && parser.parse())
    {
        cout << *options.begin() << endl;
        options.clear();
    }
}