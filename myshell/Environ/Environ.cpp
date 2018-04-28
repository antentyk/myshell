#include <vector>
#include <boost/algorithm/string.hpp>
#include <cstdlib>

#include "Environ.h"
#include "../Common/Common.h"

using namespace myshell;
using std::string;
using std::vector;
using std::set;

const char myshell::EQUALITY_SIGN = '=';

set<string> myshell::nonExportedVariables;

void myshell::exportVar(string var)
{
    auto it = nonExportedVariables.find(var);

    if(it == nonExportedVariables.end())
        return mfail
            (
                "Variable " + var + " is already exported or does not exist",
                ENVIRONMENT_FAIL
            );
    
    nonExportedVariables.erase(it);

    MERRNO = SUCCESS;
}

void myshell::setVar(string var, string value)
{
    if(setenv(var.c_str(), value.c_str(), 1) == -1)
        return mfail(
                "Error while setting " + var + " value",
                ENVIRONMENT_FAIL
            );

    nonExportedVariables.insert(var);

    MERRNO = SUCCESS;
}

void myshell::setUpChildEnviron()
{
    MERRNO = SUCCESS;

    for(auto &it: nonExportedVariables)
        if(unsetenv(it.c_str()) == -1)
            mfail(
                "Error while deleting " + it + " for child process",
                ENVIRONMENT_FAIL
            );
}