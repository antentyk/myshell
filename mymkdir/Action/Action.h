#ifndef ACTION_H_
#define ACTION_H_

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "../Settings/Settings.h"
#include "../Tokens/Tokens.h"

namespace mymkdir{
    void displayHelp();

    bool isAbsolute(const std::string &path);
    
    std::vector<std::string> split(const std::string &path);

    int run
    (
        const std::vector<std::string> &arguments,
        const Settings &settings
    );
}

#endif