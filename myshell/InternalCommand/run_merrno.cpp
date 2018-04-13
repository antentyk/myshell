#include "InternalCommand.h"

void InternalCommand::run_merrno(
    std::stringstream &strm
){

    std::multiset<std::string> options;

    Parse::InternalCommandParser parser(
        strm,
        options
    );

    int previous_merrno = MERRNO;

    MERRNO = SUCCESS;

    while(Parse::test_stream(strm))
        if(!parser.parse())
            return;
    
    if(InternalCommand::seek_for_help_token(options)){
        MERRNO = SUCCESS;
        return display_help_merrno();
    }
    
    if(options.size() > 0)
        return mfail("Too many arguments", InternalCommand::MERRNO_FAIL);
    
    std::cout << previous_merrno << std::endl;
    MERRNO = SUCCESS;
}