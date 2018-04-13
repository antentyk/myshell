#include "Action.h"

using namespace mymkdir;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::cerr;
using std::vector;

void mymkdir::displayHelp(){
    cout << "usage:" << endl;
    cout << "mymkdir [-h|--help] [-p]  <dirname>" << endl;
    cout << "Create directory if it does not already exist" << endl;
    cout << "-p no error if existing, make parent directories as needed" << endl;
}

bool mymkdir::isAbsolute(const std::string &path){
    if(path.size() == 0)
        return false;
    
    return path.at(0) == '/';
}

vector<string> mymkdir::split(const std::string &path){
    vector<string> result;
    string currentFolderName;

    for(auto current: path)
    {
        if(current == '/')
        {
            if(currentFolderName.size() != 0)
                result.push_back(currentFolderName);
            currentFolderName = "";
        }
        else
            currentFolderName.push_back(current);
    }
    if(currentFolderName.size() > 0)
        result.push_back(currentFolderName);

    return result;
}

int mymkdir::run
(
    const vector<string> &arguments,
    const Settings &settings
)
{
    if(settings.isHelp()){
        displayHelp();
        return EXIT_SUCCESS;
    }

    if(arguments.size() != 1){
        cerr << "Invalid number of arguments" << endl;
        return EXIT_FAILURE;
    }

    string path = arguments.at(0);
    vector<string> parents = split(path);

    if(!settings.isParents())
    {
        parents.clear();
        parents.push_back(path);
    }
    else
    {
        if(parents.size() == 0)
            parents.push_back(path);
        else if(isAbsolute(path))
            parents.at(0) = "/" + parents.at(0);
    
        for(size_t i = 1; i < parents.size(); ++i)
            parents.at(i) = "/" + parents.at(i);
    }

    string currentPath = "";
    for(auto it: parents)
    {
        currentPath += it;

        struct stat buffer;

        if(stat(currentPath.c_str(), &buffer) == 0)
        {
            if(!settings.isParents() || !S_ISDIR(buffer.st_mode))
            {
                cerr << "Cannot create directory " << currentPath;
                cerr << " file exists" << endl;
                return EXIT_FAILURE;
            }
            
            continue;
        }

        int success = mkdir(currentPath.c_str(), 0777);
        
        if(success == 0)
            continue;
        
        cerr << "Error while creating " << currentPath << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}