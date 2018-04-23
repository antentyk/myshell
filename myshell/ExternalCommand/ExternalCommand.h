#ifndef EXTERNALCOMMAND_H_
#define EXTERNALCOMMAND_H_

#include <string>
#include <vector>
#include <sstream>

namespace myshell{
    extern const int
        EXTERNAL_EXEC_FAIL,
        EXTERNAL_WILDCARD_FAIL,
        EXTERNAL_NO_EXECUTABLE_FAIL,
        EXTERNAL_FORK_FAIL;

    extern const std::string
        CURRENT_PATH,
        PREVIOUS_PATH;

    void execute(
        std::string command_name,
        std::vector<std::string> options
    );

    std::string get_base_name(std::string path);
    std::string get_dir_name(std::string path);
    std::string get_real_path(std::string relative_path);
    bool is_file(std::string file_name);
    bool is_executable(std::string file_name);
    bool is_directory(std::string file_name);

    bool retrieve(
        std::stringstream &strm,
        std::string * command_name,
        std::vector<std::string> &options
    );

    void execute(std::stringstream &strm);
    void seek(
        std::string folder_name,
        std::string &needed_name,
        std::vector<std::string> &possible_targets,
        bool is_recursive
    );
}

#endif