#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <dirent.h>

#include "../Settings/Settings.h"
#include "../File/File.h"
#include "../General/General.h"

namespace Display{
    class Displayer{
    public:
        Displayer
        (
            std::ostream &output,
            const Settings::SettingsHolder &settingsHolder,
            const File::FilesSorter &filesSorter
        );

        void run();
    private:
        void out_single_directory(File::FileInstance directory);
        void out_single_file(File::FileInstance file);

        void out_directory(File::FileInstance directory, bool is_name);
        
        std::ostream &output;
        const Settings::SettingsHolder &settingsHolder;
        const File::FilesSorter &filesSorter;
    };

    void get_files(
        File::FileInstance directory,
        std::vector<File::FileInstance> &target
    );

    extern const std::string
        CURRENT_PATH_INDICATOR,
        PARENT_PATH_INDICATOR;
}

#endif