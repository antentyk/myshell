#include "File.h"

using namespace File;

FilesSorter::FilesSorter(){
    options_map[SortingOptions::UNSORTED] = &UNSORTED;
    options_map[SortingOptions::SIZE] = &SIZE;
    options_map[SortingOptions::LAST_MODIFICATION_TIME] = &LAST_MODIFICATION_TIME;
    options_map[SortingOptions::EXTENSION] = &EXTENSION;
    options_map[SortingOptions::NAME] = &NAME;
}

bool FilesSorter::set(std::string option_name){
    auto itr = options_map.find(option_name);
    if(itr == options_map.end())return false;

    for(auto &it: options_map)
        *(it.second) = false;
    
    *(itr->second) = true;

    return true;
}

void FilesSorter::sort(std::vector<FileInstance> &target) const{
    if(UNSORTED)
        return;

    if(SIZE)
        return std::sort
        (
            target.begin(),
            target.end(),
            [](const FileInstance &l, const FileInstance &r)
            {
                return l.SIZE < r.SIZE;
            }
        );
    
    if(LAST_MODIFICATION_TIME)
        return std::sort
        (
            target.begin(),
            target.end(),
            [](const FileInstance &l, const FileInstance &r)
            {
                return l.LAST_MODIFICATION_TIME < r.LAST_MODIFICATION_TIME;
            }
        );
    
    if(EXTENSION)
        return std::sort
        (
            target.begin(),
            target.end(),
            [](const FileInstance &l, const FileInstance &r)
            {
                return l.EXTENSION < r.EXTENSION;
            }
        );
    
    if(NAME)
        return std::sort
        (
            target.begin(),
            target.end(),
            [](const FileInstance &l, const FileInstance &r)
            {
                return l.NAME < r.NAME;
            }
        );
}