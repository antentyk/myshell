#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <boost/algorithm/string.hpp>

#include "../Parse/Parse.h"
#include "../Wildcard/Wildcard.h"
#include "../Environ/Environ.h"

#include "ExternalCommand.h"
#include "../InternalCommand/InternalCommand.h"

using namespace myshell;
using std::string;
using std::stringstream;
using std::vector;

const std::string
    myshell::CURRENT_PATH = ".",
    myshell::PREVIOUS_PATH = "..";

void myshell::executeExternal(stringstream &strm){
    string command_name;
    vector<string> options;

    string input_redirection{""};
    string stdout_redirection{"::initvalue::"};
    string cerr_redirection{"::initvalue::"};
    bool same_stdout_and_cerr_redirection = false;

    bool background_running = false;

    if(!retrieve(
            strm,
            &command_name,
            options,
            &input_redirection,
            &stdout_redirection,
            &cerr_redirection,
            &same_stdout_and_cerr_redirection,
            &background_running
    ))
        return;
    
    string base_name = get_base_name(command_name);
    string dir_name = get_dir_name(command_name);

    vector<string> executables;

    vector<string> pathFolders;
    string pathStr = getenv("PATH");
    boost::split(pathFolders, pathStr, boost::is_any_of(":"));

    if(dir_name[0] != command_name[0])
        // need to seek in start folder
        seek(INITIAL_PATH, base_name, executables, true);

    // searching in path folders
    for(auto &folder: pathFolders)
        seek(folder + "/" + dir_name, base_name, executables, false);

    // searching in current folder
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
        if(!background_running)
            waitpid(pid, &child_exit_code, 0);
        MERRNO = child_exit_code;
    }
    else{
        vector<const char*> arg_for_c;
        arg_for_c.push_back(executable_name.c_str());
        
        for(auto &it: options)
            arg_for_c.push_back(it.c_str());
        arg_for_c.push_back(nullptr);

        setUpChildEnviron();

        if(input_redirection != ""){
            int fd = open(input_redirection.c_str(), O_RDONLY);
            if(fd == -1)
                return mfail("Cannot find input redirection file", myshell::REDIRECTION_FAIL);
            if(close(STDIN_FILENO) == -1)
                return mfail("Cannot close stdin file", myshell::REDIRECTION_FAIL);
            if(dup(fd) == -1)
                return mfail("Cannot duplicate input redirection file", myshell::REDIRECTION_FAIL);
        }

        if(stdout_redirection != "::initvalue::" || cerr_redirection != "::initvalue::"){
            if(stdout_redirection != "::initvalue::" && stdout_redirection != ""){
                int fd = open(stdout_redirection.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                if(fd == -1)
                    return mfail("Cannot open stdout redirection file", myshell::REDIRECTION_FAIL);
                if(close(STDOUT_FILENO) == -1)
                    return mfail("Cannot close stdout file", myshell::REDIRECTION_FAIL);
                if(dup(fd) == -1)
                    return mfail("Cannot duplicate stdout redirection file", myshell::REDIRECTION_FAIL);
            }
            if(cerr_redirection != "::initvalue::" && cerr_redirection != ""){
                int fd = open(cerr_redirection.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                if(fd == -1)
                    return mfail("Cannot open cerr redirection file", myshell::REDIRECTION_FAIL);
                if(close(STDERR_FILENO) == -1)
                    return mfail("Cannot close cerr file", myshell::REDIRECTION_FAIL);
                if(dup(fd) == -1)
                    return mfail("Cannot duplicate cerr redirection file", myshell::REDIRECTION_FAIL);
            }

            if(same_stdout_and_cerr_redirection){
                if(stdout_redirection == ""){
                    if(close(STDOUT_FILENO) == -1)
                        return mfail("Cannot close stdout file", myshell::REDIRECTION_FAIL);
                    if(dup(STDERR_FILENO) == -1)
                        return mfail("Cannot duplicate cerr file", myshell::REDIRECTION_FAIL);
                }
                if(cerr_redirection == ""){
                    if(close(STDERR_FILENO) == -1)
                        return mfail("Cannot close cerr file", myshell::REDIRECTION_FAIL);
                    if(dup(STDOUT_FILENO) == -1)
                        return mfail("Cannot duplicate stdout file", myshell::REDIRECTION_FAIL);
                }
            }
        }

        if(background_running){
            if(close(STDOUT_FILENO) == -1)
                return mfail("Cannot close stdout file", myshell::REDIRECTION_FAIL);
            if(close(STDERR_FILENO) == -1)
                return mfail("Cannot close cerr file", myshell::REDIRECTION_FAIL);
            if(close(STDIN_FILENO) == -1)
                return mfail("Cannot close stdin file", myshell::REDIRECTION_FAIL);
        }

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
    vector<string> &options,
    string * input_redirection,
    string * stdout_redirection,
    string * cerr_redirection,
    bool * same_stdout_and_cerr_redirection,
    bool * background_running
)
{
    strm >> std::skipws;

    char tmp; strm >> tmp;
    if(tmp == COMMENT_INDICATOR)
        return false;
    
    strm.putback(tmp);
    strm >> *(command_name);

    replaceEnvironmentVariables(*command_name);

    char current;

    BasicParser basic{strm};
    RawParser raw{strm};

    MERRNO = SUCCESS;

    while(strm >> current){
        strm.putback(current);

        if(current == COMMENT_INDICATOR)
        {
            while(strm >> current);
            break;
        }

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
            if(MERRNO != SUCCESS || current_option.front() == COMMENT_INDICATOR)
                return false;

            if(current_option == "<"){
                *input_redirection = basic.parse();

                if(MERRNO != SUCCESS || input_redirection->front() == COMMENT_INDICATOR || (*input_redirection).empty()){
                    mfail("Cannot find filename for input redirection", myshell::REDIRECTION_FAIL);
                    return false;
                }

                continue;
            }

            if(current_option == "2>&1"){
                *same_stdout_and_cerr_redirection = true;
                *cerr_redirection = "";
                continue;
            }
            if(current_option == "1>&2"){
                *same_stdout_and_cerr_redirection = true;
                *stdout_redirection = "";
                continue;
            }

            if(current_option == ">" || current_option == "2>"){
                string *inp = current_option == ">" ? stdout_redirection: cerr_redirection;
                *inp = basic.parse();

                if(MERRNO != SUCCESS || inp->front() == COMMENT_INDICATOR || (*inp).empty()){
                    mfail("Cannot find filename for output redirection", myshell::REDIRECTION_FAIL);
                    return false;
                }

                continue;
            }

            if(current_option == "&"){
                *background_running = true;
                continue;
            }
        }

        if(current != RAW_DELIMITER)
            replaceEnvironmentVariables(current_option);

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
