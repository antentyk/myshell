#include "Display.h"

using namespace Display;

void Displayer::out_single_directory(File::FileInstance directory){
    if(settingsHolder.IS_SPECIAL_FILE_TYPE)
        output << "/";
    output << directory.NAME;

    if(settingsHolder.IS_DETAILED_FILE_INFORMATION){
        output << " ";

        output << directory.SIZE << " ";
        output << General::get_date_representation(directory.LAST_MODIFICATION_TIME);
        output << std::endl;
    }
    else
        output << " ";
}