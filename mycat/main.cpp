#include <iostream>
#include <cstring>
#include <set>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>


bool ISINVISIBLECHARS;


class CharPointerCmp{
public:
    inline bool operator()(const char * l, const char * r){
        return strcmp(l, r) < 0;
    }
};
std::set<const char*, CharPointerCmp> files;


void display_help(){
    std::cout<<"Displaing help (я не встиг нормально дописати)"<<std::endl;
    std::cout<<"mycat [-h|--help] [-A] <file1> <file2> ... <fileN>"<<std::endl;
}


void fail(int errno_copy){
    std::cerr<<"Errno value: "<<errno_copy<<std::endl;
    std::cerr<<strerror(errno_copy)<<std::endl;
    exit(errno_copy);
}


void deal_argument(const char * argument){
    static const char * help_short = "-h";
    static const char * help_long = "--help";
    static const char * invisible_characters_short = "-A";

    if(strcmp(argument, help_short) == 0 || strcmp(argument, help_long) == 0){
        display_help();
        exit(EXIT_SUCCESS);
    }

    if(strcmp(argument, invisible_characters_short) == 0){
        ISINVISIBLECHARS = true;
        return;
    }

    files.insert(argument);
}


class HexDealer{
public:
    static const int REPLACEMENTAMOUNT = 4;
    static const int NONREPLACEMENTAMOUNT = 1;

    static const int HEXBASE = 16;
    static const int DECIMALBASE = 10;
    
    static inline char convert(int digit){
        if(digit < DECIMALBASE)return '0' + digit;
        digit -= DECIMALBASE;
        return 'a' + digit;
    }

    static inline bool isreplaced(char c){
        if(isspace(c) != 0)return false;
        if(isprint(c) != 0)return false;
        return true;
    }

    static inline int charsnum(char c){
        return isreplaced(c) ? REPLACEMENTAMOUNT : NONREPLACEMENTAMOUNT;
    }

    static void replace(char * &current, char c){
        if(!isreplaced(c)){
            *(current++) = c;
            return;
        }
        *(current++) = '\\';
        *(current++) = 'x';
        *(current++) = convert(c / HEXBASE);
        *(current++) = convert(c % HEXBASE);
    }
};


class MyFile{
public:

    MyFile(const char* filename){
        this->filename = filename;
        descriptor = open(filename, O_RDONLY);
        if(descriptor == -1){
            std::cerr<<"Error while opening "<<filename<<std::endl;
            fail(errno);
        }
    }

    ~MyFile(){
        int success = close(descriptor);
        if(success == -1){
            std::cerr<<"Error while closing "<<filename<<std::endl;
            fail(errno);
        }
    }
    
    void deal(){
        ssize_t bytes_read;

        while(true){
            bytes_read = read(descriptor, buffer, BUFFERSIZE);
            if(bytes_read == -1){
                int errno_copy = errno;
                if(errno_copy == EINTR)continue;
                else{
                    std::cerr<<"Error while reading "<<filename<<std::endl;
                    fail(errno_copy);
                } 
            }
            if(!bytes_read)return;

            if(!ISINVISIBLECHARS)out_buffer(buffer, buffer + bytes_read);
            else{
                ssize_t newsize = 0;
                for(ssize_t i = 0; i < bytes_read; i++)
                    newsize += HexDealer::charsnum(buffer[i]);
                char* newbuffer = (char*) malloc(sizeof(char) * newsize);
                char *current = newbuffer;
                for(ssize_t i = 0; i < bytes_read; i++)
                    HexDealer::replace(current, buffer[i]);
                out_buffer(newbuffer, current);
                delete newbuffer;
            }
        }
    }

private:
    static const int BUFFERSIZE = 1000;

    const char* filename;
    int descriptor;
    char buffer[BUFFERSIZE];

    void out_buffer(char * start, char * end){
        ssize_t written_now = 0;
        ssize_t bytes_left = end - start;

        while(bytes_left){
            written_now = write(STDOUT_FILENO, start, bytes_left);
            if(written_now == -1){
                int errno_copy = errno;
                if(errno_copy == EINTR)continue;
                else {
                    std::cerr<<"Error while writing "<<filename<<std::endl;
                    fail(errno_copy);
                }
            }
            bytes_left -= written_now;
            start += written_now;
        }
    }
};


int main(int argc, char ** argv){
    if(argc == 1){
        std::cerr<<"No arguments provided"<<std::endl;
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i < argc; i++)
        deal_argument(*(++argv));
    
    for(auto &it: files){
        MyFile f(it);
        f.deal();
    }

    exit(EXIT_SUCCESS);
}