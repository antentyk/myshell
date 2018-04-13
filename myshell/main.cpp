#include <string>
#include <string.h>
#include <sstream>
#include <limits.h>
#include <unistd.h>

#include "ExternalCommand/ExternalCommand.h"
#include "InternalCommand/InternalCommand.h"
#include "Tokens/Tokens.h"

using namespace std;

std::string get_current_path(){
    static char BUFFER[PATH_MAX];
    char *result = getcwd(BUFFER, PATH_MAX);
    if(result == NULL){
        mfail("Error while getting current path", EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    return std::string(result);
}

void execute(){
    std::cout << get_current_path() << " $ ";

    std::string line;
    getline(std::cin, line);

    std::stringstream strm(line);

    std::string command_name;
    strm >> std::skipws;
    strm >> command_name;

    if(strcmp(command_name.c_str(), Tokens::MERRNO.c_str()) == 0)
        return InternalCommand::run_merrno(strm);
    if(strcmp(command_name.c_str(), Tokens::MEXIT.c_str()) == 0)
        return InternalCommand::run_mexit(strm);
    if(strcmp(command_name.c_str(), Tokens::MPWD.c_str()) == 0)
        return InternalCommand::run_mpwd(strm);
    if(strcmp(command_name.c_str(), Tokens::MCD.c_str()) == 0)
        return InternalCommand::run_mcd(strm);
    
    strm.seekg(0, std::ios::beg);
    return ExternalCommand::execute(strm);
}

int main(){
    while(true)
        execute();
    return 0;
}