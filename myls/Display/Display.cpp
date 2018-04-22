#include <string>
#include <dirent.h>
#include <algorithm>

#include "../General/General.h"
#include "Display.h"

using namespace myls;
using std::string;
using std::ostream;
using std::vector;
using std::reverse;
using std::copy_if;
using std::remove_if;
using std::distance;
using std::endl;

const string
    myls::CURRENT_PATH_INDICATOR{"."},
    myls::PARENT_PATH_INDICATOR{".."};

Displayer::Displayer
(
    ostream &output,
    const SettingsHolder &settingsHolder,
    const FilesSorter &filesSorter
):
    output(output),
    settingsHolder(settingsHolder),
    filesSorter(filesSorter)
{}

void Displayer::out_directory(FileInstance directory, bool is_name){
    vector<FileInstance> files;
    get_files(directory, files);
    filesSorter.sort(files);
    if(settingsHolder.IS_REVERSED_ORDER)
        reverse(files.begin(), files.end());

    if(is_name)
        output << directory.PATH << ":" << endl;

    auto is_directory = [](const FileInstance &file){
        return file.IS_DIRECTORY;
    };
    auto is_special = [](const FileInstance &file){
        return !file.IS_ALL_THE_REST && !file.IS_DIRECTORY;
    };

    vector<FileInstance> sub_directories(files.size());
    auto sub_directories_end = copy_if
    (
        files.begin(),
        files.end(),
        sub_directories.begin(),
        is_directory
    );
    sub_directories.resize(distance(sub_directories.begin(), sub_directories_end));

    vector<FileInstance> special_files(files.size());
    auto special_files_end = copy_if
    (
        files.begin(),
        files.end(),
        special_files.begin(),
        is_special
    );
    special_files.resize(distance(special_files.begin(), special_files_end));

    if(settingsHolder.IS_DIRECTORIES_FIRST){
        files.erase
        (
            remove_if(files.begin(), files.end(), is_directory),
            files.end()
        );

        for(auto &it: sub_directories)
            out_single_directory(it);
        
        output << endl;
    }

    if(settingsHolder.IS_SPECIAL_FILES){
        files.erase(
            remove_if(files.begin(), files.end(), is_special),
            files.end()
        );

        for(auto &it: special_files)
            out_single_file(it);
        
        output << endl;
    }

    for(auto it: files)
        if(it.IS_FILE)
            out_single_file(it);
        else
            out_single_directory(it);
    output << endl;
    
    if(settingsHolder.IS_RECURSIVE)
        for(auto it: sub_directories)
            out_directory(it, true);
}

void Displayer::out_single_directory(FileInstance directory){
    if(settingsHolder.IS_SPECIAL_FILE_TYPE)
        output << "/";
    output << directory.NAME;

    if(settingsHolder.IS_DETAILED_FILE_INFORMATION){
        output << " ";

        output << directory.SIZE << " ";
        output << get_date_representation(directory.LAST_MODIFICATION_TIME);
        output << endl;
    }
    else
        output << " ";
}

void Displayer::out_single_file(FileInstance file){
    if(settingsHolder.IS_SPECIAL_FILE_TYPE){
        if(file.IS_EXECUTABLE)
            output << "*";
        else if(file.IS_NAMED_PIPE)
            output << "|";
        else if(file.IS_SYMLINK)
            output << "@";
        else if(file.IS_SOCKET)
            output << "=";
        else
            output << "?";
    }

    output << file.NAME;

    if(settingsHolder.IS_DETAILED_FILE_INFORMATION){
        output << " ";

        output << file.SIZE << " ";
        output << get_date_representation(file.LAST_MODIFICATION_TIME);
        output << endl;
    }
    else
        output << " ";
}

int Displayer::run(){
    vector<FileInstance> files;

    bool wasFail = false;

    for(auto &it: settingsHolder.file_names){
        FileInstance current;

        if(FileInstance::get(it, &current))
            files.push_back(current);
        else
        {
            output << "No such a file: " << it << endl;
            wasFail = true;
        }
    }

    filesSorter.sort(files);
    if(settingsHolder.IS_REVERSED_ORDER)
        reverse(files.begin(), files.end());
    
    for(auto it: files){
        if(it.IS_FILE)
            out_single_file(it);
        else
            out_directory(it, files.size() > 1 || settingsHolder.IS_RECURSIVE);
        output << endl;
    }

    return wasFail? EXIT_FAILURE : EXIT_SUCCESS; 
}

void myls::get_files
(
    FileInstance directory,
    vector<FileInstance> &target
)
{
    struct dirent **all_files;

    int n = scandir(directory.PATH.c_str(), &all_files, NULL, alphasort);

    if(n == -1)return;

    for(int i = 0; i < n; i++){
        FileInstance current;
        
        string file_name{all_files[i]->d_name};
        if
        (
            file_name == CURRENT_PATH_INDICATOR ||
            file_name == PARENT_PATH_INDICATOR
        )
            continue;
        string file_name_with_path = directory.PATH + "/" + file_name;
        
        if(FileInstance::get(file_name_with_path, &current))
            target.push_back(current);
    }

    for(int i = n - 1; i >= 0; i--)
        free(all_files[i]);
    free(all_files);
}
