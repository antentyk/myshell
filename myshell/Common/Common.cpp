#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "../Parse/Parse.h"
#include "../ExternalCommand/ExternalCommand.h"
#include "../InternalCommand/InternalCommand.h"
#include "../Tokens/Tokens.h"
#include "../Environ/Environ.h"

#include "Common.h"

using std::string;
using std::vector;
using std::deque;
using std::stringstream;
using std::cerr;
using std::endl;
using namespace myshell;

void mfail(string message, int exit_code){
    cerr << message << endl;
    ::MERRNO = exit_code;
}

namespace
{
    string init(){
        char BUFFER[PATH_MAX];
        char* result = getcwd(BUFFER, PATH_MAX);

        if(result == NULL){
            cerr << "Error while getting initial path" << endl;
            exit(EXIT_FAILURE);
        }

        return string(result);
    }
}

const string INITIAL_PATH = init();

const int
    SUCCESS = 0,
    myshell::PARSE_TOKENS_FAIL = 1,
    myshell::EXTERNAL_EXEC_FAIL = 2,
    myshell::EXTERNAL_WILDCARD_FAIL = 3,
    myshell::EXTERNAL_NO_EXECUTABLE_FAIL = 4,
    myshell::EXTERNAL_FORK_FAIL = 5,
    myshell::MERRNO_FAIL = 6,
    myshell::MPWD_FAIL = 7,
    myshell::MCD_FAIL = 8,
    myshell::MEXIT_FAIL = 9,
    myshell::SCRIPT_FAIL = 10,
    myshell::ENVIRONMENT_FAIL = 11,
    myshell::MEXPORT_FAIL = 12,
    myshell::REDIRECTION_FAIL = 13,
    myshell::PIPE_FAIL = 14;

int MERRNO = SUCCESS;

void execute(string line){
    if(line.find(myshell::PIPE_INDICATOR) != std::string::npos)
        return execute_pipe(line);

    stringstream strm(line);

    string command_name;
    strm >> std::skipws;
    strm >> command_name;

    if(command_name == myshell::MERRNO)
        return run_merrno(strm);
    if(command_name == MEXIT)
        return run_mexit(strm);
    if(command_name == MPWD)
        return run_mpwd(strm);
    if(command_name == MCD)
        return run_mcd(strm);
    if(command_name == MSCRIPT)
        return run_script(strm);
    if(command_name == MEXPORT)
        return run_mexport(strm);
    if(command_name == MECHO)
        return run_mecho(strm);
    
    strm.seekg(0, std::ios::beg);

    string variableIndicator = string(1, ESCAPE_CHARACTER_INDICATOR) + EQUALITY_SIGN;

    if(!boost::contains(command_name, string(1, EQUALITY_SIGN)))
        return executeExternal(strm);
    if(!boost::contains(command_name, variableIndicator))
        return run_variable(strm);
    
    string tmp{strm.str()};

    boost::replace_first(tmp, variableIndicator, string(1, EQUALITY_SIGN));
    strm.str(tmp);
    return executeExternal(strm);
}

int externalScript(int argc, char ** argv)
{
    if(argc != 2)
    {
        mfail("Invalid number of arguments", SCRIPT_FAIL);
        return ::MERRNO;
    }

    string scriptName{argv[1]};
    stringstream strm{scriptName};

    run_script(strm);

    return ::MERRNO;
}

namespace{
    void perform_pipe(
            deque<string> commands,
            int terminal_STDIN,
            int terminal_STDOUT,
            int terminal_STDERR)
    {
        if(commands.size() == 1){
            dup2(terminal_STDOUT, STDOUT_FILENO);
            dup2(terminal_STDERR, STDERR_FILENO);

            execute(commands.front());

            dup2(terminal_STDIN, STDIN_FILENO);
            return;
        }

        int pipe_fd[2];
        pipe(pipe_fd);

        int pipe_in = pipe_fd[0];
        int pipe_out = pipe_fd[1];

        string command = commands.front();
        commands.pop_front();

        pid_t pid = fork();
        if(pid == -1){
            dup2(terminal_STDERR, STDERR_FILENO);
            return mfail("Error while fork", PIPE_FAIL);
        }
        if(pid > 0){
            //parent
            close(pipe_in);

            dup2(pipe_out, STDOUT_FILENO);
            dup2(pipe_out, STDERR_FILENO);

            execute(command);

            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            close(pipe_out);

            int child_exit_code;
            waitpid(pid, &child_exit_code, 0);
            ::MERRNO = child_exit_code;
        }
        else{
            // child
            close(pipe_out);

            dup2(pipe_in, STDIN_FILENO);

            close(pipe_in);

            return perform_pipe(commands, terminal_STDIN, terminal_STDOUT, terminal_STDERR);
        }
    }
}

void execute_pipe(string line){
    vector<string> commandsV;
    boost::split(commandsV, line, boost::is_any_of(std::string(1, myshell::PIPE_INDICATOR)));
    deque<string> commands;

    std::copy(commandsV.begin(), commandsV.end(), std::back_inserter(commands));

    int saved_STDIN = dup(STDIN_FILENO);
    int saved_STDOUT = dup(STDOUT_FILENO);
    int saved_STDERR = dup(STDERR_FILENO);

    perform_pipe(commands, saved_STDIN, saved_STDOUT, saved_STDERR);

    dup2(saved_STDIN, STDIN_FILENO);
    dup2(saved_STDOUT, STDOUT_FILENO);
    dup2(saved_STDERR, STDERR_FILENO);

    return;
}