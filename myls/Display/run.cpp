#include "Display.h"

using namespace Display;

void Displayer::run(){
    std::vector<File::FileInstance> files;

    for(auto &it: settingsHolder.file_names){
        File::FileInstance current;

        if(File::FileInstance::get(it, &current))
            files.push_back(current);
    }

    filesSorter.sort(files);
    if(settingsHolder.IS_REVERSED_ORDER)
        std::reverse(files.begin(), files.end());
    
    for(auto it: files){
        if(it.IS_FILE)
            out_single_file(it);
        else
            out_directory(it, files.size() > 1 || settingsHolder.IS_RECURSIVE);
        output << std::endl;
    } 
}