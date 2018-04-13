#include "File.h"

using namespace File;

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
    std::string name,
    FileInstance *target
)
{
    struct stat stat_buf;
    if(stat(name.c_str(), &stat_buf) != 0)
        return false;
    
    target->IS_DIRECTORY = S_ISDIR(stat_buf.st_mode);
    target->IS_FILE = !target->IS_DIRECTORY;

    while(name[name.size() - 1] == '/')
        name.pop_back();
    target->PATH = name;
    
    char* name_buffer = new char[name.size() + 1];
    strcpy(name_buffer, name.c_str());
    target->NAME = std::string(basename(name_buffer));
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