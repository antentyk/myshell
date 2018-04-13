#include "Parser.h"

using namespace mymv;
using std::string;
using std::vector;

void Parser::parse(
    Settings &settings,
    std::vector<std::string> &arguments,
    int argc,
    char **argv
)
{
    --argc; ++argv;

    for(; argc > 0; --argc, ++argv)
    {
        string currentArgument(*argv);

        if(!settings.setArgument(currentArgument))
            arguments.push_back(currentArgument);
    }
}