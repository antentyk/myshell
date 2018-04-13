#ifndef FILE_H_
#define FILE_H_

#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>

#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../SortingOptions/SortingOptions.h"

namespace File{
    class FileInstance{
    public:
        FileInstance& operator=(FileInstance other);

        static bool get
        (
            std::string name,
            FileInstance *target
        ); // returns true if file exists and fills target info

        std::string
            PATH = "", // ./mydirectory/tmp or ./mydirectory/myfile.txt
            NAME = "", // tmp or myfile.txt
            EXTENSION = ""; // --nothing-- or txt

        bool
            IS_FILE = false,
            IS_DIRECTORY = false,
            IS_EXECUTABLE = false,
            IS_SYMLINK = false,
            IS_NAMED_PIPE = false,
            IS_SOCKET = false,
            IS_ALL_THE_REST = false;
        
        size_t SIZE = 0;
        off_t LAST_MODIFICATION_TIME = 0;
    };

    class FilesSorter{
    public:
        FilesSorter();

        bool
            UNSORTED = false,
            SIZE = false,
            LAST_MODIFICATION_TIME = false,
            EXTENSION = false,
            NAME = true;

        std::map<std::string, bool*> options_map;
        
        bool set(std::string option_name);
        void sort(std::vector<FileInstance> &target) const;
    };
}

#endif