#include "Action.h"

using namespace myrm;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::cerr;
using std::vector;
using std::set;

void myrm::displayHelp(){
    cout << "usage:" << endl;
    cout << "myrm [-h|--help] [-f] [-R] <file1> <file2> <file3>" << endl << endl;
    cout << "Asks before deleting of each file" << endl;
    cout << "Possible answers: " << endl;
    cout << "Y[es]/N[o]/A[ll]/C[ancel]" << endl << endl;
    cout << "-f then you won't be asked at all" << endl;
    cout << "-R to remove directories (either empty or not empty)" << endl;
}

bool myrm::clearDirectory(string dirName){
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

string myrm::confirm(string prompt){
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

int myrm::run
(
    const vector<string> &arguments,
    const Settings &settings
)
{
    if(settings.isHelp()){
        displayHelp();
        return EXIT_SUCCESS;
    }

    if(arguments.size() == 0){
        cerr << "Invalid number of arguments" << endl;
        return EXIT_FAILURE;
    }

    bool wasFailure = false;
    bool askAnyMore = !settings.isForced();

    for(auto &file_name: arguments){
        struct stat buffer;

        if(stat(file_name.c_str(), &buffer) != 0){
            cerr << "No file named " << file_name << endl;
            wasFailure = true;
            continue;
        }

        if(S_ISDIR(buffer.st_mode) && !settings.isRecursive()){
            cerr << "Cannot remove directory ";
            cerr << "(run with " << kRecursive << ")" << endl;
            wasFailure = true;
            continue;
        }

        bool needAsk = askAnyMore;
        if(needAsk)
        {
            string response = confirm(
                "Do you really want to delete " +
                file_name
            );

            if(response == kNo)
                continue;
            if(response == kCancel)
                return EXIT_SUCCESS;
            if(response == kAll)
                askAnyMore = false;
        }

        if(S_ISREG(buffer.st_mode) && unlink(file_name.c_str()) != 0)
        {
            cerr << "Error while deleting " << file_name << endl;
            wasFailure = true;
            continue;
        }

        if(S_ISDIR(buffer.st_mode) && !clearDirectory(file_name))
        {
            cerr << "Error while deleting " << file_name << endl;
            wasFailure = true;
            continue;
        }
    }

    return wasFailure? EXIT_FAILURE : EXIT_SUCCESS;
}