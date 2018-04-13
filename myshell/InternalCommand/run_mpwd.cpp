#include "InternalCommand.h"

void InternalCommand::run_mpwd(
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
        return display_help_mpwd();
    }

    if(options.size() > 0)
        return mfail("Too many options", InternalCommand::MPWD_FAIL);
    
    MERRNO = SUCCESS;
    
    static char BUFFER[PATH_MAX];
    char *result = getcwd(BUFFER, PATH_MAX);

    if(result == NULL)
        return mfail(
            "Error occured while getting current path",
            InternalCommand::MPWD_FAIL
        );
    
    std::cout << result << std::endl;
}