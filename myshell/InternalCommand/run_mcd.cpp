#include "InternalCommand.h"

void InternalCommand::run_mcd(
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
        return display_help_merrno();
    }

    if(options.size() == 0)
        return mfail(
            "Too few options",
            InternalCommand::MCD_FAIL
        );
    
    if(options.size() > 1)
        return mfail(
            "Too many options",
            InternalCommand::MCD_FAIL
        );
    
    std::string new_path = *(options.begin());
    int success = chdir(new_path.c_str());

    if(success < 0)
        return mfail("Invalid path", InternalCommand::MCD_FAIL);

    MERRNO = SUCCESS;
}