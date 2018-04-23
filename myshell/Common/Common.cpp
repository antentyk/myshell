#include <iostream>
#include <limits.h>
#include <unistd.h>

#include "../Parse/Parse.h"
#include "../ExternalCommand/ExternalCommand.h"
#include "../InternalCommand/InternalCommand.h"

#include "Common.h"

using std::string;
using std::cerr;
using std::endl;

void mfail(string message, int exit_code){
    cerr << message << endl;
    MERRNO = exit_code;
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
    myshell::MEXIT_FAIL = 9;

int MERRNO = SUCCESS;