#ifndef ACTION_H_
#define ACTION_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <exception>
#include <stack>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

#include "../Settings/Settings.h"
#include "../Tokens/Tokens.h"

namespace mycp{
    void displayHelp();

    std::string confirm(std::string prompt);

    // it is guaranteed that dest is not an existing direcrtory
    int copySingleFile(std::string source, std::string dest, const Settings &settings);

    std::string get_basename(std::string full_name);

    // it is guaranteed that destDirName is an existing directory
    int copyToDirectory(
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