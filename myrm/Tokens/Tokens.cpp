#include "Tokens.h"

using namespace myrm;
using std::string;
using std::set;

const string
    myrm::kHelpLong = "--help",
    myrm::kHelpShort = "-h",
    myrm::kForced = "-f",
    myrm::kRecursive = "-R";

const string
    myrm::kYes = "yes",
    myrm::kNo = "no",
    myrm::kAll = "all",
    myrm::kCancel = "cancel";

const set<string>
    myrm::kReplacementAnswers{
        kYes,
        kNo,
        kAll,
        kCancel
    };