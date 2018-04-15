#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

#include "../Settings/Settings.h"

namespace mycp{
    class Parser{
    public:
        static void parse
        (
            Settings &settings,
            std::vector<std::string> &arguments,
            int argc,
            char **argv
        );
    };
}

#endif