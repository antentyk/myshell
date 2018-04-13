#include <iostream>
#include <string>
#include <vector>

#include "Parser/Parser.h"
#include "Settings/Settings.h"
#include "Action/Action.h"

using namespace mymkdir;
using std::vector;
using std::string;
using std::cerr;
using std::endl;


int main(int argc, char **argv){
    if(argc == 0){
        cerr << "Invalid number of arguments" << endl;
        return EXIT_FAILURE;
    }

    Settings settings;
    vector<string> arguments;

    Parser::parse(settings, arguments, argc, argv);

    return run(arguments, settings);
}