#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>

#include "../Parse/Parse.h"
#include "../Wildcard/Wildcard.h"

#include "ExternalCommand.h"

using namespace myshell;
using std::string;
using std::stringstream;
using std::vector;

const std::string
    myshell::CURRENT_PATH = ".",
    myshell::PREVIOUS_PATH = "..";

void myshell::execute(stringstream &strm){
    string command_name;
    vector<string> options;

    if(!retrieve(strm, &command_name, options))
        return;
    
    string base_name = get_base_name(command_name);
    string dir_name = get_dir_name(command_name);

    vector<string> executables;

    if(dir_name[0] != command_name[0])
        // need to seek in start folder
        seek(INITIAL_PATH, base_name, executables, true);

    seek(dir_name, base_name, executables, false);

    for(size_t i = 0; i < executables.size(); i++)
        executables[i] = get_real_path(executables[i]);
    
    if(executables.size() == 0)
        return mfail(
            "No executables found",
            EXTERNAL_NO_EXECUTABLE_FAIL
        );

    string executable_name = executables[0];

    pid_t pid = fork();
    
    if(pid == -1)
        return mfail("Failed to fork", EXTERNAL_FORK_FAIL);
    if(pid > 0){
        int child_exit_code;
        waitpid(pid, &child_exit_code, 0);
        MERRNO = child_exit_code;
    }
    else{
        vector<const char*> arg_for_c;
        arg_for_c.push_back(executable_name.c_str());
        
        for(auto &it: options)
            arg_for_c.push_back(it.c_str());
        arg_for_c.push_back(nullptr);

        execvp(
            executable_name.c_str(),
            const_cast<char* const*>(arg_for_c.data())
        );

        mfail(
            "Failed to execute " + executable_name,
            EXTERNAL_EXEC_FAIL
        );

        exit(EXTERNAL_EXEC_FAIL);
    }
}

string myshell::get_base_name(
    string path
)
{
    char* temp = new char[path.size() + 1];
    temp[path.size()] = '\0';
    strcpy(temp, path.c_str());

    char* result = basename(temp);

    string answer(result);

    delete [] temp;

    return answer;
}

string myshell::get_dir_name(
    string path
)
{
    char* temp = new char[path.size() + 1];
    temp[path.size()] = '\0';
    strcpy(temp, path.c_str());

    char* result = dirname(temp);

    string a(result);

    delete [] temp;

    return a;
}

string myshell::get_real_path(string relative_path){
    char BUFFER[PATH_MAX];

    realpath(relative_path.c_str(), BUFFER);

    return string(BUFFER);
}

bool myshell::is_directory(string file_name){
    struct stat path_stat;
    if(stat(file_name.c_str(), &path_stat) != 0)
        return false;
    return S_ISDIR(path_stat.st_mode);
}

bool myshell::is_executable(string file_name){
    struct stat path_stat;
    if(stat(file_name.c_str(), &path_stat) != 0)
        return false;
    return path_stat.st_mode & S_IXUSR;
}

bool myshell::is_file(string file_name){
    struct stat path_stat;
    if(stat(file_name.c_str(), &path_stat) != 0)
        return false;
    return S_ISREG(path_stat.st_mode);
}

bool myshell::retrieve(
    stringstream &strm,
    string * command_name,
    vector<string> &options
)
{
    strm >> std::skipws;
    strm >> *(command_name);

    char current;

    BasicParser basic{strm};
    RawParser raw{strm};

    MERRNO = SUCCESS;

    while(strm >> current){
        strm.putback(current);
        string current_option;

        if(current == RAW_DELIMITER){
            current_option = raw.parse(RAW_DELIMITER);
            if(MERRNO != SUCCESS)
                return false;
            options.push_back(current_option);
            continue;
        }
        else if(current == WHITESPACE_DELIMITER){
            current_option = raw.parse(WHITESPACE_DELIMITER);
            if(MERRNO != SUCCESS)
                return false;
        }
        else{
            current_option = basic.parse();
            if(MERRNO != SUCCESS)
                return false;
        }

        string dir_name = get_dir_name(current_option);
        string base_name = get_base_name(current_option);

        if(!is_wildcard(base_name)){
            options.push_back(current_option);
            continue;
        }

        WildcardTransformer wildcard_checker{base_name};

        struct dirent **files_storage;
        int n = scandir(
            dir_name.c_str(),
            &files_storage,
            NULL,
            alphasort
        );

        if(n == -1){
            mfail(
                "Erorr while opening " + get_real_path(dir_name),
                EXTERNAL_WILDCARD_FAIL
            );
            return false;
        }
            
        int currently_added = 0;

        while(n--){
            string file_name = string(files_storage[n]->d_name);
            free(files_storage[n]);
    
            if(!is_file(dir_name + "/" +file_name))continue;
            if(!wildcard_checker.match(file_name))continue;

            options.push_back(dir_name + "/" + file_name);
            currently_added++;
        }
        free(files_storage);

        if(currently_added == 0){
            mfail(
                "No file match the wildcard " + current_option,
                EXTERNAL_WILDCARD_FAIL
            );
            return false;
        }
    }
    
    return true;
}

void myshell::seek(
    string folder_name,
    string &needed_name,
    vector<string> &possible_targets,
    bool is_recursive
){
    struct dirent **all_files;
    int n = scandir(folder_name.c_str(), &all_files, NULL, alphasort);
    
    if(n == -1)
        return;
    
    for(int i = 0; i < n; i++){
        string current_name = all_files[i]->d_name;
        if(is_directory(folder_name + "/" + current_name))
            continue;
        if(!is_executable(folder_name + "/" + current_name))
            continue;
        if(current_name != needed_name)
            continue;
        possible_targets.push_back(folder_name + "/" + current_name);
    }

    if(is_recursive){
        for(int i = 0; i < n; i++){
            string current_name = all_files[i]->d_name;
            if(!is_directory(folder_name + "/" + current_name))
                continue;
            if(current_name == PREVIOUS_PATH || current_name == CURRENT_PATH)
                continue;
            seek(
                folder_name + "/" + current_name,
                needed_name,
                possible_targets,
                is_recursive
            );
        }
    }

    for(int i = n - 1; i >= 0; i--)
        free(all_files[i]);
    free(all_files);
}
