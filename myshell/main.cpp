#include <string>
#include <string.h>
#include <sstream>
#include <limits.h>
#include <unistd.h>
#include <iostream>

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

void execute(){
    cout << get_current_path() << " $ ";

    string line;
    getline(cin, line);

    stringstream strm(line);

    string command_name;
    strm >> std::skipws;
    strm >> command_name;

    if(strcmp(command_name.c_str(), myshell::MERRNO.c_str()) == 0)
        return run_merrno(strm);
    if(strcmp(command_name.c_str(), MEXIT.c_str()) == 0)
        return run_mexit(strm);
    if(strcmp(command_name.c_str(), MPWD.c_str()) == 0)
        return run_mpwd(strm);
    if(strcmp(command_name.c_str(), MCD.c_str()) == 0)
        return run_mcd(strm);
    
    strm.seekg(0, std::ios::beg);
    return execute(strm);
}

int main(){
    while(true)
        execute();
    return 0;
}