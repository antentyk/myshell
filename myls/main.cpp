#include <iostream>
#include <cstdlib>
#include <ostream>

#include "Display/Display.h"
#include "Parse/Parse.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "General/General.h"

int main(int argc, char** argv){
    ++argv;

    Settings::SettingsHolder settingsHolder;

    File::FilesSorter filesSorter;

    Parse::OptionsParser parser(
        argv,
        settingsHolder,
        filesSorter
    );
    parser.parse();
    if(settingsHolder.IS_HELP){
        General::display_help(std::cout);
        exit(EXIT_SUCCESS);
    }
    if(settingsHolder.IS_INVALID_OPTION){
        std::cout << "Invalid option detected" << std::endl;
        exit(EXIT_FAILURE);
    }

    Display::Displayer displayer(
        std::cout,
        settingsHolder,
        filesSorter
    );

    displayer.run();

    exit(EXIT_SUCCESS);
}