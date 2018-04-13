#include "Display.h"

using namespace Display;

void Displayer::out_single_file(File::FileInstance file){
    if(settingsHolder.IS_SPECIAL_FILE_TYPE){
        if(file.IS_EXECUTABLE)
            output << "* ";
        else if(file.IS_NAMED_PIPE)
            output << "| ";
        else if(file.IS_SYMLINK)
            output << "@ ";
        else if(file.IS_SOCKET)
            output << "= ";
        else
            output << "? ";
    }

    output << file.NAME;

    if(settingsHolder.IS_DETAILED_FILE_INFORMATION){
        output << " ";

        output << file.SIZE << " ";
        output << General::get_date_representation(file.LAST_MODIFICATION_TIME);
        output << std::endl;
    }
    else
        output << " ";
}