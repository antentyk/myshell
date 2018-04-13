#include "InternalCommand.h"

void InternalCommand::run_mexit(
    std::stringstream &strm
){
    std::multiset<std::string> options;

    Parse::InternalCommandParser parser(
        strm,
        options
    );

    MERRNO = SUCCESS;

    while(Parse::test_stream(strm))
        if(!parser.parse())
            return;
    
    if(InternalCommand::seek_for_help_token(options)){
        MERRNO = SUCCESS;
        return display_help_mexit();
    }

    if(options.size() > 1)
        return mfail("Too many options", InternalCommand::MEXIT_FAIL);

    int exit_code;
    if(options.size() == 0)
        exit_code = SUCCESS;
    else{
        std::stringstream tmp; 
        tmp << *(options.begin());
        tmp >> exit_code;
        if(tmp.fail())
            return mfail(
                "Invalid exit code",
                InternalCommand::MEXIT_FAIL
            );
    }
    
    MERRNO = SUCCESS;
    exit(exit_code);
}