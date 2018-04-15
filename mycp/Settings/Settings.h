#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <map>

#include "../Tokens/Tokens.h"

namespace mycp{
    class Settings{
    public:
        Settings();

        inline bool isHelp() const { return isHelp_; }
        inline bool isForced() const { return isForced_; }
        inline bool isRecursive() const { return isRecursive_; }

        bool setArgument(std::string argument);
    private:
        bool
            isHelp_,
            isForced_,
            isRecursive_;
        
        std::map<std::string, bool*> settingsMap_;
    };
}

#endif