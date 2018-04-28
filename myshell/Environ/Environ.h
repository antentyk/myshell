#ifndef ENVIRON_H_
#define ENVIRON_H_

#include <set>

namespace myshell
{
    extern const int ENVIRONMENT_FAIL;

    extern const char EQUALITY_SIGN;

    extern std::set<std::string> nonExportedVariables;
    
    void setUpChildEnviron();
    void exportVar(std::string var);
    void setVar(std::string var, std::string value);
}

#endif