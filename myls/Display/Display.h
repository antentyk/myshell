#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <iostream>
#include <vector>

#include "../Settings/Settings.h"
#include "../File/File.h"

namespace myls{
    class Displayer{
    public:
        Displayer
        (
            std::ostream &output,
            const SettingsHolder &settingsHolder,
            const FilesSorter &filesSorter
        );

        int run();
    private:
        void out_single_directory(FileInstance directory);
        void out_single_file(FileInstance file);

        void out_directory(FileInstance directory, bool is_name);
        
        std::ostream &output;
        const SettingsHolder &settingsHolder;
        const FilesSorter &filesSorter;
    };

    void get_files(
        FileInstance directory,
        std::vector<FileInstance> &target
    );

    extern const std::string
        CURRENT_PATH_INDICATOR,
        PARENT_PATH_INDICATOR;
}

#endif