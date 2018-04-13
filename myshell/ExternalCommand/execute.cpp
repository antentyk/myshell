#include "ExternalCommand.h"

using namespace ExternalCommand;

void ExternalCommand::execute(std::stringstream &strm){
    std::string command_name;
    std::vector<std::string> options;

    if(!retrieve(strm, &command_name, options))
        return;
    
    std::string base_name = get_base_name(command_name);
    std::string dir_name = get_dir_name(command_name);

    std::vector<std::string> executables;

    if(dir_name[0] != command_name[0])
        // need to seek in start folder
        ExternalCommand::seek(INITIAL_PATH, base_name, executables, true);

    ExternalCommand::seek(dir_name, base_name, executables, false);

    for(size_t i = 0; i < executables.size(); i++)
        executables[i] = ExternalCommand::get_real_path(executables[i]);
    
    if(executables.size() == 0)
        return mfail(
            "No executables found",
            ExternalCommand::EXTERNAL_NO_EXECUTABLE_FAIL
        );

    std::string executable_name = executables[0];

    pid_t pid = fork();
    
    if(pid == -1)
        return mfail("Failed to fork", ExternalCommand::EXTERNAL_FORK_FAIL);
    if(pid > 0){
        int child_exit_code;
        waitpid(pid, &child_exit_code, 0);
        MERRNO = child_exit_code;
    }
    else{
        std::vector<const char*> arg_for_c;
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
            ExternalCommand::EXTERNAL_EXEC_FAIL
        );

        exit(ExternalCommand::EXTERNAL_EXEC_FAIL);
    }
}