#include "Action.h"

using namespace mymv;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::cerr;
using std::vector;
using std::set;

void mymv::displayHelp(){
    cout << "usage:" << endl;
    cout << "mymv [-h|--help] [-f] <oldfile> <newfile>" << endl;
    cout << "   if newfile exists, asks if you want to replace it or not" << endl;
    cout << "mymv [-h|--help] [-f] <oldfile_or_dir_1> <oldfile_or_dir_oldfile2> <oldfile_or_dir_oldfile3>.... <dir>" << endl;
    cout << "   if some of oldfile_or_dir_1 or other exists, asks if you want to replace it or not" << endl;
    cout << "Possible answers: Y[es]/N[o]/A[ll]/C[ancel]" << endl;
    cout << "It is case insensitive" << endl;
}

bool mymv::clearDirectory(string dirName){
    // from http://man7.org/linux/man-pages/man3/scandir.3.html example
    struct dirent **namelist;

    int n = scandir(dirName.c_str(), &namelist, NULL, alphasort);
    if(n == -1)
        return false;
    
    bool wasFailure = false;

    while(n--)
    {
        if(wasFailure)
        {
            free(namelist[n]);
            continue;
        }

        string current_name = namelist[n]->d_name;
        if(current_name == ".." || current_name == ".")
            continue;
        current_name = dirName + "/" + current_name;

        struct stat buffer;
        if(stat(current_name.c_str(), &buffer) != 0)
            wasFailure = true;
        
        free(namelist[n]);

        if(wasFailure)
            continue;

        if(S_ISDIR(buffer.st_mode))
        {
            if(!clearDirectory(current_name))
                wasFailure = true;
        }
        else
        {
            if(unlink(current_name.c_str()) != 0)
                wasFailure = true;
        }
    }
    free(namelist);

    if(wasFailure)
        return false;
    else
        return rmdir(dirName.c_str()) == 0;
}

string mymv::get_basename(string full_name){
    char p[PATH_MAX];
    strcpy(p, full_name.c_str());

    return string(basename(p));
}

string mymv::getReplacementResponse(string prompt){
    cout << prompt << endl;
    cout << "Y[es]/N[o]/A[ll]/C[ancel]" << endl;

    string answer;
    while(cin >> answer){
        std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
        for(auto &it: kReplacementAnswers){
            if(answer == it)
                return it;
            if(answer.size() == 1 && it.at(0) == answer.at(0))
                return it;
        }

        cout << "Invalid input. Try again" << endl;
    }

    throw std::runtime_error("Unknown exception occured");
}

int mymv::moveSingleFile
(
    string sourceName,
    string destName,
    const Settings &settings
)
{
    struct stat
        sourceBuffer,
        destBuffer;

    if(stat(sourceName.c_str(), &sourceBuffer) != 0){
        cerr << sourceName << " does not exist";
        return EXIT_FAILURE;
    }

    if(stat(destName.c_str(), &destBuffer) == 0)
    {
        bool needAsk = !settings.isForced();

        if(needAsk)
        {
            string answer = getReplacementResponse(
                "Do you really want to replace " +
                destName +
                "?"
            );

            if(answer == kNo || answer == kCancel)
                return EXIT_SUCCESS;
        }

        remove(destName.c_str());
    }

    int success = rename(sourceName.c_str(), destName.c_str());

    if(success != 0)
        cerr << "Error occured while renaming " << sourceName << endl;
    
    return success;
}

int mymv::moveToDirectory
(
    const std::vector<std::string> &arguments,
    std::string destDirName,
    const Settings &settings
)
{
    bool askAnyMore = !settings.isForced();
    set<string> currentlyReplaced;

    for(auto &it: arguments)
    {   
        string old_name = it;
        string base_name = get_basename(it);
        string new_name = destDirName + "/" + base_name;

        if(currentlyReplaced.find(base_name) != currentlyReplaced.end()){
            cerr << "mymv will not replace just created " << base_name << endl;
            continue;
        }

        struct stat buffer;
        if(stat(new_name.c_str(), &buffer) != 0){
            int success = rename(old_name.c_str(), new_name.c_str());
            if(success == 0)
                continue;
            cerr << "Error while moving " << old_name << endl;
            return EXIT_FAILURE;
        }

        bool needAsk = askAnyMore;
        if(needAsk)
        {
            string response = getReplacementResponse(
                "Do you want to replace " +
                base_name
            );

            if(response == kNo)
                continue;
            if(response == kCancel)
                return EXIT_SUCCESS;
            if(response == kAll)
                askAnyMore = false;
        }

        if(S_ISDIR(buffer.st_mode)){
            if(!clearDirectory(new_name))
            {
                cerr << "Error while clearing " << base_name << endl;
                return EXIT_FAILURE;
            }
        }
        else{
            if(unlink(new_name.c_str()) != 0)
            {
                cerr << "Error while deleting " << base_name << endl;
                return EXIT_FAILURE;
            }
        }
        
        if(rename(old_name.c_str(), new_name.c_str()) != 0)
        {
            cerr << "Error while moving " << base_name << endl;
            return EXIT_FAILURE;
        }

        currentlyReplaced.insert(base_name);
    }

    return EXIT_SUCCESS;
}

int mymv::run
(
    const vector<string> &arguments,
    const Settings &settings
)
{
    if(settings.isHelp()){
        displayHelp();
        return EXIT_SUCCESS;
    }

    if(arguments.size() <= 1){
        cerr << "Invalid number of arguments (at least 2 expected)" << endl;
        return EXIT_FAILURE;
    }

    struct stat buffer;

    if(stat(arguments.back().c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode))
    {
        vector<string> newArguments = arguments;
        string dirName = newArguments.back();
        newArguments.pop_back();

        return moveToDirectory(newArguments, dirName, settings);
    }
    if(arguments.size() == 2)
        return moveSingleFile(arguments.at(0), arguments.at(1), settings);
    
    cerr << "No rule found for provided arguments" << endl;
    return EXIT_FAILURE;
}
