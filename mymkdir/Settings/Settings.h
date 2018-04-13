#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <map>

#include "../Tokens/Tokens.h"

namespace mymkdir{
    class Settings{
    public:
        Settings();

        inline bool isHelp() const { return isHelp_; }
        inline bool isParents() const { return isParents_; }

        bool setArgument(std::string argument);
    private:
        bool
            isHelp_,
            isParents_;
        
        std::map<std::string, bool*> settingsMap_;
    };
}

#endif