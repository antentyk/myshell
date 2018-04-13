#include "Display.h"

using namespace Display;

void Displayer::out_directory(File::FileInstance directory, bool is_name){
    std::vector<File::FileInstance> files;
    Display::get_files(directory, files);
    filesSorter.sort(files);
    if(settingsHolder.IS_REVERSED_ORDER)
        std::reverse(files.begin(), files.end());

    if(is_name)
        output << directory.PATH << ":" << std::endl;

    auto is_directory = [](File::FileInstance file){
        return file.IS_DIRECTORY;
    };
    auto is_special = [](File::FileInstance file){
        return !file.IS_ALL_THE_REST;
    };

    std::vector<File::FileInstance> sub_directories(files.size());
    auto sub_directories_end = std::copy_if
    (
        files.begin(),
        files.end(),
        sub_directories.begin(),
        is_directory
    );
    sub_directories.resize(std::distance(sub_directories.begin(), sub_directories_end));

    std::vector<File::FileInstance> special_files(files.size());
    auto special_files_end = std::copy_if
    (
        files.begin(),
        files.end(),
        special_files.begin(),
        is_special
    );
    special_files.resize(std::distance(special_files.begin(), special_files_end));

    if(settingsHolder.IS_DIRECTORIES_FIRST){
        files.erase
        (
            std::remove_if(files.begin(), files.end(), is_directory),
            files.end()
        );

        for(auto &it: sub_directories)
            out_single_directory(it);
        
        output << std::endl;
    }

    if(settingsHolder.IS_SPECIAL_FILES){
        files.erase(
            std::remove_if(files.begin(), files.end(), is_special),
            files.end()
        );

        for(auto &it: special_files)
            out_single_file(it);
        
        output << std::endl;
    }

    for(auto it: files)
        if(it.IS_FILE)
            out_single_file(it);
        else
            out_single_directory(it);
    output << std::endl;
    
    if(settingsHolder.IS_RECURSIVE)
        for(auto it: sub_directories)
            out_directory(it, true);
}