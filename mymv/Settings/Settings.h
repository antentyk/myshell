#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <map>

#include "../Tokens/Tokens.h"

namespace mymv{
    class Settings{
    public:
        Settings();

        inline bool isHelp() const { return isHelp_; }
        inline bool isForced() const { return isForced_; }

        bool setArgument(std::string argument);
    private:
        bool
            isHelp_,
            isForced_;
        
        std::map<std::string, bool*> settingsMap_;
    };
}

#endif