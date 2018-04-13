#ifndef GENERAL_H_
#define GENERAL_H_

#include <ostream>
#include <string>
#include <ctime>

namespace General{
    void display_help(std::ostream &output);

    std::string get_date_representation(time_t timestamp);
}

#endif