#include "Common.h"

int MERRNO = SUCCESS;

void mfail(std::string message, int exit_code){
    std::cerr << message << std::endl;
    MERRNO = exit_code;
}

std::string init(){
    char BUFFER[PATH_MAX];
    char* result = getcwd(BUFFER, PATH_MAX);

    if(result == NULL){
        std::cerr << "Error while getting initial path" << std::endl;
        exit(EXIT_FAILURE);
    }

    return std::string(result);
}

const std::string INITIAL_PATH = init();