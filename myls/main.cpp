#include <iostream>
#include <cstdlib>

#include "Display/Display.h"
#include "Parse/Parse.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "General/General.h"

using namespace myls;

int main(int argc, char** argv){
    ++argv;

    SettingsHolder settingsHolder;

    FilesSorter filesSorter;

    OptionsParser parser(
        argv,
        settingsHolder,
        filesSorter
    );
    parser.parse();
    if(settingsHolder.IS_HELP){
        display_help(std::cout);
        exit(EXIT_SUCCESS);
    }
    if(settingsHolder.IS_INVALID_OPTION){
        std::cout << "Invalid option detected" << std::endl;
        exit(EXIT_FAILURE);
    }

    Displayer displayer(
        std::cout,
        settingsHolder,
        filesSorter
    );

    return displayer.run();
}