#ifndef INTERNAL_COMMAND_H_
#define INTERNAL_COMMAND_H_

#include <string>
#include <set>
#include <sstream>

#include <limits.h>
#include <unistd.h>

namespace myshell{
    extern const int
        MERRNO_FAIL,
        MPWD_FAIL,
        MCD_FAIL,
        MEXIT_FAIL,
        SCRIPT_FAIL;
    
    void display_help_tokens();
    void display_help_merrno();
    void display_help_mpwd();
    void display_help_mcd();
    void display_help_mexit();
    void display_help_script();


    void run_merrno(std::stringstream &strm);
    void run_mexit(std::stringstream &strm);
    void run_mpwd(std::stringstream &strm);
    void run_mcd(std::stringstream &strm);
    void run_script(std::stringstream &strm);
    bool seek_for_help_token(std::multiset<std::string> &target);
}

#endif