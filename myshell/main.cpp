#include <string>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <iostream>

#include <cstdlib>

#include "ExternalCommand/ExternalCommand.h"
#include "InternalCommand/InternalCommand.h"
#include "Tokens/Tokens.h"
#include "Common/Common.h"

using namespace std;
using namespace myshell;
using std::string;
using std::cout;
using std::cin;
using std::endl;

string get_current_path(){
    char BUFFER[PATH_MAX];
    char *result = getcwd(BUFFER, PATH_MAX);
    if(result == NULL){
        mfail("Error while getting current path", EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    return string(result);
}

int main(int argc, char ** argv){
    if(argc > 1)
        return externalScript(argc, argv);

    while(true)
    {
        cout << "\033[1;31m" << get_current_path() << "$ " << "\033[0m";
        string line;
        getline(cin, line);
        execute(line);
    }
    return 0;
}