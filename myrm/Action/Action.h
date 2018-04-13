#ifndef ACTION_H_
#define ACTION_H_

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <exception>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "../Settings/Settings.h"
#include "../Tokens/Tokens.h"

namespace myrm{
    void displayHelp();

    // it is guaranteed that dirName is an existing directory
    bool clearDirectory(std::string dirName);

    std::string confirm(std::string prompt);

    int run
    (
        const std::vector<std::string> &arguments,
        const Settings &settings
    );
}

#endif