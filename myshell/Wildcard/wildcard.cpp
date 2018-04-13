#include "Wildcard.h"

std::set<char> init(){
    std::set<char> result;

    result.insert('[');
    result.insert(']');
    result.insert('*');
    result.insert('?');

    return result;
}

const std::set<char>
    Wildcard::WILDCARD_INDICATORS = init();