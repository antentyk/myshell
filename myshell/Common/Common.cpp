#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <sstream>

#include "../Parse/Parse.h"
#include "../ExternalCommand/ExternalCommand.h"
#include "../InternalCommand/InternalCommand.h"
#include "../Tokens/Tokens.h"

#include "Common.h"

using std::string;
using std::stringstream;
using std::cerr;
using std::endl;
using namespace myshell;

void mfail(string message, int exit_code){
    cerr << message << endl;
    ::MERRNO = exit_code;
}

namespace
{
    string init(){
        char BUFFER[PATH_MAX];
        char* result = getcwd(BUFFER, PATH_MAX);

        if(result == NULL){
            cerr << "Error while getting initial path" << endl;
            exit(EXIT_FAILURE);
        }

        return string(result);
    }
}

const string INITIAL_PATH = init();

const int
    SUCCESS = 0,
    myshell::PARSE_TOKENS_FAIL = 1,
    myshell::EXTERNAL_EXEC_FAIL = 2,
    myshell::EXTERNAL_WILDCARD_FAIL = 3,
    myshell::EXTERNAL_NO_EXECUTABLE_FAIL = 4,
    myshell::EXTERNAL_FORK_FAIL = 5,
    myshell::MERRNO_FAIL = 6,
    myshell::MPWD_FAIL = 7,
    myshell::MCD_FAIL = 8,
    myshell::MEXIT_FAIL = 9,
    myshell::SCRIPT_FAIL = 10;

int MERRNO = SUCCESS;

void execute(string line){
    stringstream strm(line);

    string command_name;
    strm >> std::skipws;
    strm >> command_name;

    if(command_name == myshell::MERRNO)
        return run_merrno(strm);
    if(command_name == MEXIT)
        return run_mexit(strm);
    if(command_name == MPWD)
        return run_mpwd(strm);
    if(command_name == MCD)
        return run_mcd(strm);
    if(command_name == MSCRIPT)
        return run_script(strm);
    
    strm.seekg(0, std::ios::beg);
    return executeExternal(strm);
}

int externalScript(int argc, char ** argv)
{
    if(argc != 2)
    {
        mfail("Invalid number of arguments", SCRIPT_FAIL);
        return ::MERRNO;
    }

    string scriptName{argv[1]};
    stringstream strm{scriptName};

    run_script(strm);

    return ::MERRNO;
}