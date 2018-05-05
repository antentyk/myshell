#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <regex>
#include <memory>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::endl;
using std::cerr;

const std::string
    kHelpShort = "-h",
    kHelpLong = "--help",
    kInvertMatchShort = "-v",
    kInvertMatchLong = "--invert-match",
    kIgnoreCaseShort = "-i",
    kIgnoreCaseLong = "--ignore-case",
    kFileLong = "--file",
    kRegexpLong = "--regexp";

void displayHelp(){
    cout << "Displaying help (не встигаю нормально дописати)" << endl;
}

class ParseError : public std::exception{
public:
    const char *what() const throw(){
        return "Error while parsing (run with --help)";
    }
};

class UnrecognizedOption : public ParseError{
public:
    const char *what() const throw(){
        return "Cannot recognize option (run with --help)";
    }
};

class Settings{
public:
    inline void setHelp() noexcept { help = true; }
    inline void setInvertMatch() noexcept { invertMatch = true; }
    inline void setIgnoreCase() noexcept { ignoreCase = true; }

    inline void setFilename(const std::string &val) noexcept { filename = val; }
    inline void setRegexp(const std::string &val) noexcept { regexp = val; }

    inline bool getHelp() const noexcept { return help; }
    inline bool getInvertMatch() const noexcept { return invertMatch; }
    inline bool getIgnoreCase() const noexcept { return ignoreCase; }

    inline const std::string& getRegexp()const noexcept{ return regexp; }
    inline const std::string& getFilename() const noexcept{ return filename; }
private:
    bool
        help{false},
        invertMatch{false},
        ignoreCase{false};
    std::string
        regexp{""},
        filename{""};
};

void parse(int argc, char **argv, Settings &result){
    if(argc == 0)
        return;

    --argc;
    ++argv;

    while(argc--){
        std::string line{*(argv++)};

        if(line == kHelpLong || line == kHelpShort)
            result.setHelp();
        else if(line == kInvertMatchLong || line == kInvertMatchShort)
            result.setInvertMatch();
        else if(line == kIgnoreCaseLong || line == kIgnoreCaseShort)
            result.setIgnoreCase();
        else{
            if(boost::starts_with(line, kFileLong + "="))
                result.setFilename(line.substr(kFileLong.size() + 1));
            else if(boost::starts_with(line, kRegexpLong + "="))
                result.setRegexp(line.substr(kRegexpLong.size() + 1));
            else if(boost::starts_with(line, "-"))
                throw UnrecognizedOption();
            else
                result.setRegexp(".*" + line + ".*");
        }
    }
}

class RegexpMatcher{
public:
    RegexpMatcher(const Settings &settings):
        ignoreCase(settings.getIgnoreCase()),
        invertMatch(settings.getInvertMatch()),
        expression(
            ignoreCase?
            std::regex(settings.getRegexp(), std::regex_constants::ECMAScript | std::regex_constants::icase) :
            std::regex(settings.getRegexp())
        )
    {}

    inline bool match(const std::string &candidate){
        return static_cast<bool>(regex_match(candidate, expression)) ^ invertMatch;
    }

private:
    bool ignoreCase;
    bool invertMatch;
    std::regex expression;
};

class Executor{
public:
    Executor(std::istream &strm, const Settings &settings):
        strm(strm),
        regexpMatcher(RegexpMatcher(settings))
    {}

    void run(){
        std::string line;

        while(getline(strm, line))
            if(regexpMatcher.match(line))
                cout << line << endl;
    }

private:
    std::istream &strm;
    RegexpMatcher regexpMatcher;
};

int main(int argc, char ** argv){
    Settings settings;

    try{
        parse(argc, argv, settings);
    }
    catch(const ParseError &e){
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    if(settings.getHelp()){
        displayHelp();
        return EXIT_SUCCESS;
    }

    if(settings.getRegexp() == ""){
        cerr << "Cannot find regexp" << endl;
        return EXIT_FAILURE;
    }

    if(settings.getFilename() == "")
        Executor(std::cin, settings).run();
    else{
        std::ifstream file;
        file.open(settings.getFilename());

        if(!file.is_open()){
            cerr << "Cannot find " << settings.getFilename() << endl;
            return EXIT_FAILURE; 
        }

        Executor(file, settings).run();
    }

    return EXIT_SUCCESS;
}