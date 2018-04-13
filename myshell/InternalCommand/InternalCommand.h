#ifndef INTERNAL_COMMAND_H_
#define INTERNAL_COMMAND_H_

#include <string>
#include <limits.h>
#include <unistd.h>

#include "../Common/Common.h"
#include "../Parse/Parse.h"
#include "../Tokens/Tokens.h"

namespace InternalCommand{
    extern const int
        MERRNO_FAIL,
        MPWD_FAIL,
        MCD_FAIL,
        MEXIT_FAIL;
    
    void display_help_tokens();
    void display_help_merrno();
    void display_help_mpwd();
    void display_help_mcd();
    void display_help_mexit();


    void run_merrno(std::stringstream &strm);
    void run_mexit(std::stringstream &strm);
    void run_mpwd(std::stringstream &strm);
    void run_mcd(std::stringstream &strm);
    bool seek_for_help_token(std::multiset<std::string> &target);
}

#endif