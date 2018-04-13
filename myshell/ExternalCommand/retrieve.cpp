#include "ExternalCommand.h"

using namespace ExternalCommand;

bool ExternalCommand::retrieve(
    std::stringstream &strm,
    std::string * command_name,
    std::vector<std::string> &options
)
{
    strm >> std::skipws;
    strm >> *(command_name);

    char current;

    Parse::BasicParser basic{strm};
    Parse::RawParser raw{strm};

    MERRNO = SUCCESS;

    while(strm >> current){
        strm.putback(current);
        std::string current_option;

        if(current == Parse::RAW_DELIMITER){
            current_option = raw.parse(Parse::RAW_DELIMITER);
            if(MERRNO != SUCCESS)
                return false;
            options.push_back(current_option);
            continue;
        }
        else if(current == Parse::WHITESPACE_DELIMITER){
            current_option = raw.parse(Parse::WHITESPACE_DELIMITER);
            if(MERRNO != SUCCESS)
                return false;
        }
        else{
            current_option = basic.parse();
            if(MERRNO != SUCCESS)
                return false;
        }

        std::string dir_name = get_dir_name(current_option);
        std::string base_name = get_base_name(current_option);

        if(!Wildcard::is_wildcard(base_name)){
            options.push_back(current_option);
            continue;
        }

        Wildcard::WildcardTransformer wildcard_checker{base_name};

        struct dirent **files_storage;
        int n = scandir(
            dir_name.c_str(),
            &files_storage,
            NULL,
            alphasort
        );

        if(n == -1){
            mfail(
                "Erorr while opening " + get_real_path(dir_name),
                EXTERNAL_WILDCARD_FAIL
            );
            return false;
        }
            
        int currently_added = 0;

        while(n--){
            std::string file_name = std::string(files_storage[n]->d_name);
            free(files_storage[n]);
    
            if(!is_file(dir_name + "/" +file_name))continue;
            if(!wildcard_checker.match(file_name))continue;

            options.push_back(dir_name + "/" + file_name);
            currently_added++;
        }
        free(files_storage);

        if(currently_added == 0){
            mfail(
                "No file match the wildcard " + current_option,
                EXTERNAL_WILDCARD_FAIL
            );
            return false;
        }
    }
    
    return true;
}