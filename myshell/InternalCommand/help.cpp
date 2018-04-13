#include "InternalCommand.h"

void InternalCommand::display_help_tokens(){
    std::cout << "[";
    std::cout << Tokens::HELP_SHORT;
    std::cout << "|";
    std::cout << Tokens::HELP_LONG;
    std::cout << "]";
}

void InternalCommand::display_help_merrno(){
    std::cout << Tokens::MERRNO << " ";

    InternalCommand::display_help_tokens();
    std::cout << std::endl;

    std::cout << "Displays end code of the last command executed";
    std::cout << std::endl;

    std::cout << "If no commands were executed, displays ";
    std::cout << SUCCESS << std :: endl;
}

void InternalCommand::display_help_mpwd(){
    std::cout << Tokens::MPWD << " ";

    InternalCommand::display_help_tokens();
    std::cout << std::endl;

    std::cout << "Displays the current path" << std::endl;
}

void InternalCommand::display_help_mcd(){
    std::cout << Tokens::MCD;

    std::cout << " <path> ";

    InternalCommand::display_help_tokens();
    std::cout << std::endl;

    std::cout << "Navigates to the provided directory" << std::endl;
}

void InternalCommand::display_help_mexit(){
    std::cout << Tokens::MEXIT;

    std::cout << " <exit code> ";

    InternalCommand::display_help_tokens();
    std::cout << std::endl;

    std::cout << "Terminates myshell session with given exit code";
    std::cout << std::endl;

    std::cout << "If no code is procided, exits with ";
    std::cout << SUCCESS << std::endl;
}