#include "General.h"

std::string General::get_date_representation(time_t timestamp){
    std::tm * ptm = std::localtime(&timestamp);
    char buffer[32];
    std::strftime(buffer, 32, "%d.%m.%Y %H:%M:%S", ptm);

    return std::string(buffer);
}