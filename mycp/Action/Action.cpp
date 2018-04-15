#include "Action.h"

using namespace mycp;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::cerr;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::streamsize;
using std::stack;
using std::pair;
using std::make_pair;

namespace
{
    bool copyFile(const string &sourceFileName, const string &destFileName)
    {   
        static const streamsize kBufferSize = 4096;
        char buffer[kBufferSize];

        ifstream source(sourceFileName, ifstream::binary | ifstream::in);
        ofstream dest(destFileName, ofstream::binary | ofstream::out);

        if(!source.is_open() || !dest.is_open())
            goto error;

        while(true)
        {
            source.read(buffer, kBufferSize);
            
            if(source.gcount() == 0)
                break;
            
            dest.write(buffer, source.gcount());
        }

        struct stat sourceBuffer;
        stat(sourceFileName.c_str(), &sourceBuffer);
        
        chown(destFileName.c_str(), sourceBuffer.st_uid, sourceBuffer.st_gid);
        chmod(destFileName.c_str(), sourceBuffer.st_mode);

        goto success;

        error:
        {
            source.close();
            dest.close();
            return false;
        }
        success:
        {
            source.close();
            dest.close();
            return true;
        }
    }
}

void mycp::displayHelp(){
    cout << "Displaying help" << endl;
}

string mycp::confirm(string prompt){
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

int mycp::run
(
    const vector<string> &arguments,
    const Settings &settings
)
{
    if(settings.isHelp()){
        displayHelp();
        return EXIT_SUCCESS;
    }

    if(arguments.size() == 0)
    {
        cerr << "Invalid number of arguments" << endl;
        return EXIT_FAILURE;
    }

    struct stat buffer;

    if(stat(arguments.back().c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode))
    {
        vector<string> newArguments = arguments;
        newArguments.pop_back();

        return copyToDirectory(newArguments, arguments.back(), settings);
    }

    if(arguments.size() == 2)
        return copySingleFile(arguments.at(0), arguments.at(1), settings);
    
    cerr << "No suitable rule found" << endl;
    return EXIT_FAILURE;
}

int mycp::copySingleFile(string source, string dest, const Settings &settings)
{
    struct stat sourceBuffer, destBuffer;

    if(stat(source.c_str(), &sourceBuffer) != 0)
    {
        cerr << "Cannot find " << source << endl;
        return EXIT_FAILURE;
    }

    if(S_ISDIR(sourceBuffer.st_mode))
    {
        cerr << "Cannot copy directory (run with " << kRecursive << ")" << endl;
        return EXIT_FAILURE;
    }

    if(stat(dest.c_str(), &destBuffer) == 0)
    {
        if
        (
            sourceBuffer.st_dev == destBuffer.st_dev && 
            sourceBuffer.st_ino == destBuffer.st_ino
        )
        {
            cerr << "Cannot copy " << source << " to the same file" << endl;
            return EXIT_FAILURE;
        }

        bool needAsk = !settings.isForced();

        if(needAsk)
        {
            string response = confirm(
                "Do you really want to replace " + 
                dest + 
                "?"
            );

            if(response == kCancel || response == kNo)
                return EXIT_FAILURE;
        }
    }

    if(!copyFile(source, dest))
    {
        cerr << "Error while copying " << source << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

string mycp::get_basename(string full_name){
    char p[PATH_MAX];
    strcpy(p, full_name.c_str());

    return string(basename(p));
}

int mycp::copyToDirectory(
    const std::vector<std::string> &arguments,
    std::string destDirName,
    const Settings &settings
)
{
    bool wasFail = false;
    bool askAnyMore = !settings.isForced();

    stack<pair<string, string>> s;

    for(auto it: arguments)
        s.push(make_pair(it, destDirName)); // file, directory
    
    while(!s.empty())
    {
        string sourceFileName = s.top().first;
        string destDirName = s.top().second;

        s.pop();

        struct stat
            destDirNameBuffer,
            sourceFileNameBuffer,
            newFileNameBuffer;

        if(stat(destDirName.c_str(), &destDirNameBuffer) != 0)
        {
            wasFail = true;
            cerr << "No such a directory: " << destDirName << endl;
            continue;
        }

        if(stat(sourceFileName.c_str(), &sourceFileNameBuffer) != 0)
        {
            wasFail = true;
            cerr << "Cannot find " << sourceFileName << endl;
            continue;
        }
        if(S_ISDIR(sourceFileNameBuffer.st_mode) && !settings.isRecursive())
        {
            wasFail = true;
            cerr << 
            "Cannot copy directory (run with " <<
            kRecursive << ")" << endl;
            continue;
        }

        string newFileName = destDirName + "/" + get_basename(sourceFileName);
        
        if(stat(newFileName.c_str(), &newFileNameBuffer) == 0)
        {
            if(!S_ISDIR(newFileNameBuffer.st_mode))
            {
                if
                (
                    sourceFileNameBuffer.st_dev == newFileNameBuffer.st_dev &&
                    sourceFileNameBuffer.st_ino == newFileNameBuffer.st_ino
                )
                {
                    wasFail = true;
                    cerr << "Cannot copy " << sourceFileName << " to the same file" << endl;
                    continue;
                }

                bool needAsk = askAnyMore;

                if(needAsk)
                {
                    string response = confirm(
                        "Do you really want to replace " +
                        newFileName
                    );

                    if(response == kCancel)
                        return wasFail? EXIT_FAILURE: EXIT_SUCCESS;
                    if(response == kNo)
                        continue;
                    if(response == kAll)
                        askAnyMore = false;
                }
            }
        }
        else
        {
            if(S_ISDIR(sourceFileNameBuffer.st_mode))
            {
                if
                (
                    mkdir(newFileName.c_str(),
                    sourceFileNameBuffer.st_mode) != 0
                )
                {
                    wasFail = true;
                    cerr << "Erorr while creating " << newFileName << endl;
                    continue;
                }
            }
        }

        if(!S_ISDIR(sourceFileNameBuffer.st_mode))
        {
            if(!copyFile(sourceFileName, newFileName))
            {
                wasFail = true;
                cerr << "Error while copying " << sourceFileName << endl;
                continue;
            }
        }
        else
        {
            // from http://man7.org/linux/man-pages/man3/scandir.3.html example
            struct dirent **namelist;

            int n = scandir(sourceFileName.c_str(), &namelist, NULL, alphasort);
            if(n == -1)
            {
                wasFail = true;
                cerr << "Error while scanning " << sourceFileName << endl;
                continue;
            }

            while(n--)
            {
                string currentName = namelist[n]->d_name;

                free(namelist[n]);

                if(currentName == ".." || currentName == ".")
                    continue;
                
                s.push(
                    make_pair(
                        sourceFileName + "/" + currentName,
                        newFileName
                    )
                );
            }
            free(namelist);
        }
    }

    return wasFail? EXIT_FAILURE: EXIT_SUCCESS;
}