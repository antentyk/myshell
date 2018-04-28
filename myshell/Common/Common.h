#ifndef COMMON_H_
#define COMMON_H_

#include <string>

extern int
    MERRNO;

extern const int
    SUCCESS;

extern const std::string INITIAL_PATH;

void mfail(std::string message, int exit_code);

void execute(std::string line);

int externalScript(int argc, char ** argv);

#endif