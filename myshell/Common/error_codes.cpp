#include "../Parse/Parse.h"
#include "../ExternalCommand/ExternalCommand.h"
#include "../InternalCommand/InternalCommand.h"

const int
    SUCCESS = 0,
    Parse::PARSE_TOKENS_FAIL = 1,
    ExternalCommand::EXTERNAL_EXEC_FAIL = 2,
    ExternalCommand::EXTERNAL_WILDCARD_FAIL = 3,
    ExternalCommand::EXTERNAL_NO_EXECUTABLE_FAIL = 4,
    ExternalCommand::EXTERNAL_FORK_FAIL = 5,
    InternalCommand::MERRNO_FAIL = 6,
    InternalCommand::MPWD_FAIL = 7,
    InternalCommand::MCD_FAIL = 8,
    InternalCommand::MEXIT_FAIL = 9;