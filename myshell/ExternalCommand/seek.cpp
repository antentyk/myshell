#include "ExternalCommand.h"

void ExternalCommand::seek(
    std::string folder_name,
    std::string &needed_name,
    std::vector<std::string> &possible_targets,
    bool is_recursive
){
    struct dirent **all_files;
    int n = scandir(folder_name.c_str(), &all_files, NULL, alphasort);
    
    if(n == -1)
        return;
    
    for(int i = 0; i < n; i++){
        std::string current_name = all_files[i]->d_name;
        if(ExternalCommand::is_directory(folder_name + "/" + current_name))
            continue;
        if(!ExternalCommand::is_executable(folder_name + "/" + current_name))
            continue;
        if(current_name != needed_name)
            continue;
        possible_targets.push_back(folder_name + "/" + current_name);
    }

    if(is_recursive){
        for(int i = 0; i < n; i++){
            std::string current_name = all_files[i]->d_name;
            if(!ExternalCommand::is_directory(folder_name + "/" + current_name))
                continue;
            if(current_name == ExternalCommand::PREVIOUS_PATH)
                continue;
            if(current_name == ExternalCommand::CURRENT_PATH)
                continue;
            ExternalCommand::seek(
                folder_name + "/" + current_name,
                needed_name,
                possible_targets,
                is_recursive);
        }
    }

    for(int i = n - 1; i >= 0; i--)
        free(all_files[i]);
    free(all_files);
}