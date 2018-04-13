#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <iostream>
#include <limits.h>
#include <unistd.h>

extern int
    MERRNO;

extern const int
    SUCCESS;

extern const std::string INITIAL_PATH;

void mfail(std::string message, int exit_code);

#endif