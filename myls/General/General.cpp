#include <string>

#include "General.h"

using std::ostream;
using std::string;

void myls::display_help(ostream &output){
    output << "Displaying help" << std::endl;
}

string myls::get_date_representation(time_t timestamp){
    std::tm * ptm = std::localtime(&timestamp);
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);

    return string(buffer);
}