#include "Tokens.h"

using namespace mycp;
using std::string;
using std::set;

const string
    mycp::kHelpLong = "--help",
    mycp::kHelpShort = "-h",
    mycp::kForced = "-f",
    mycp::kRecursive = "-R";

const string
    mycp::kYes = "yes",
    mycp::kNo = "no",
    mycp::kAll = "all",
    mycp::kCancel = "cancel";

const set<string>
    mycp::kReplacementAnswers{
        kYes,
        kNo,
        kAll,
        kCancel
    };