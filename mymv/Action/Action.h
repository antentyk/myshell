#ifndef ACTION_H_
#define ACTION_H_

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <exception>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <libgen.h>
#include <limits.h>

#include "../Settings/Settings.h"
#include "../Tokens/Tokens.h"

namespace mymv{
    void displayHelp();

    // it is guaranteed that dirName is an existing directory
    bool clearDirectory(std::string dirName);

    std::string get_basename(std::string full_name);

    std::string getReplacementResponse(std::string prompt);

    // it is guaranteed that destName is not a directory
    int moveSingleFile
    (
        std::string sourceName,
        std::string destName,
        const Settings &settings
    );

    // it is guaranteed that destDirName is existing directory
    int moveToDirectory
    (
        const std::vector<std::string> &arguments,
        std::string destDirName,
        const Settings &settings
    );

    int run
    (
        const std::vector<std::string> &arguments,
        const Settings &settings
    );
}

#endif