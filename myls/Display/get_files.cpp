#include "Display.h"

using namespace Display;

void Display::get_files
(
    File::FileInstance directory,
    std::vector<File::FileInstance> &target
)
{
    struct dirent **all_files;

    int n = scandir(directory.PATH.c_str(), &all_files, NULL, alphasort);

    if(n == -1)return;

    for(int i = 0; i < n; i++){
        

        File::FileInstance current;
        
        std::string file_name = std::string(all_files[i]->d_name);
        if
        (
            file_name == CURRENT_PATH_INDICATOR ||
            file_name == PARENT_PATH_INDICATOR
        )
            continue;
        std::string file_name_with_path = directory.PATH + "/" + file_name;
        
        if(File::FileInstance::get(file_name_with_path, &current))
            target.push_back(current);
    }

    for(int i = n - 1; i >= 0; i--)
        free(all_files[i]);
    free(all_files);
}