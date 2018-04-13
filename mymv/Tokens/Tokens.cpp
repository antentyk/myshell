#include "Tokens.h"

using namespace mymv;
using std::string;
using std::set;

const string
    mymv::kHelpLong = "--help",
    mymv::kHelpShort = "-h",
    mymv::kForced = "-f";

const string
    mymv::kYes = "yes",
    mymv::kNo = "no",
    mymv::kAll = "all",
    mymv::kCancel = "cancel";

const set<string>
    mymv::kReplacementAnswers{
        kYes,
        kNo,
        kAll,
        kCancel
    };