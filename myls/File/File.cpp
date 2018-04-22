#include <string.h>
#include <algorithm>

#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../SortingOptions/SortingOptions.h"
#include "File.h"

using namespace myls;
using std::string;
using std::vector;

FilesSorter::FilesSorter(){
    options_map[myls::UNSORTED] = &UNSORTED;
    options_map[myls::SIZE] = &SIZE;
    options_map[myls::LAST_MODIFICATION_TIME] = &LAST_MODIFICATION_TIME;
    options_map[myls::EXTENSION] = &EXTENSION;
    options_map[myls::NAME] = &NAME;
}

bool FilesSorter::set(string option_name){
    auto itr = options_map.find(option_name);
    if(itr == options_map.end())return false;

    for(auto &it: options_map)
        *(it.second) = false;
    
    *(itr->second) = true;

    return true;
}

void FilesSorter::sort(vector<FileInstance> &target) const{
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
                string lName = l.NAME, rName = r.NAME;

                std::transform(lName.begin(), lName.end(), lName.begin(), ::tolower);
                std::transform(rName.begin(), rName.end(), rName.begin(), ::tolower);

                return lName < rName;
            }
        );
}

FileInstance& FileInstance::operator=(FileInstance other){
    if(this == &other)return *this;

    this->PATH = other.PATH;
    this->NAME = other.NAME;
    this->EXTENSION = other.EXTENSION;

    this->IS_FILE = other.IS_FILE,
    this->IS_DIRECTORY = other.IS_DIRECTORY,
    this->IS_EXECUTABLE = other.IS_EXECUTABLE,
    this->IS_SYMLINK = other.IS_SYMLINK,
    this->IS_NAMED_PIPE = other.IS_NAMED_PIPE,
    this->IS_SOCKET = other.IS_SOCKET,
    this->IS_ALL_THE_REST = other.IS_ALL_THE_REST;

    this->SIZE = other.SIZE;
    this->LAST_MODIFICATION_TIME = other.LAST_MODIFICATION_TIME;

    return *this;
}

bool FileInstance::get
(
    string name,
    FileInstance *target
)
{
    struct stat stat_buf;
    if(stat(name.c_str(), &stat_buf) != 0)
        return false;
    
    target->IS_DIRECTORY = S_ISDIR(stat_buf.st_mode);
    target->IS_FILE = !target->IS_DIRECTORY;

    while(name.back() == '/')
        name.pop_back();
    target->PATH = name;
    
    char* name_buffer = new char[name.size() + 1];
    strcpy(name_buffer, name.c_str());
    target->NAME = string(basename(name_buffer));
    delete [] name_buffer;

    size_t last_dot_position = name.find_last_of('.');
    if
    (
        target->IS_FILE &&
        last_dot_position != std::string::npos
    )
        target->EXTENSION = name.substr(last_dot_position);
    
    target->LAST_MODIFICATION_TIME = stat_buf.st_mtime;
    target->SIZE = stat_buf.st_size;

    if(target->IS_FILE){
        target->IS_SOCKET = S_ISSOCK(stat_buf.st_mode);
        target->IS_SYMLINK = S_ISLNK(stat_buf.st_mode);
        target->IS_NAMED_PIPE = S_ISFIFO(stat_buf.st_mode);
        target->IS_EXECUTABLE = stat_buf.st_mode & S_IXUSR;

        target->IS_ALL_THE_REST = 
            !target->IS_SOCKET &&
            !target->IS_SYMLINK &&
            !target->IS_NAMED_PIPE &&
            !target->IS_EXECUTABLE;
    }

    return true;
}