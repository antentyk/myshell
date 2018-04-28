cmake_minimum_required(VERSION 3.5)
project(myshell)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

FIND_PACKAGE( Boost REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(mycat mycat/main.cpp)
add_executable(
    myls
    myls/Display/Display.h
    myls/Display/Display.cpp
    myls/File/File.h
    myls/File/File.cpp
    myls/General/General.h
    myls/General/General.cpp
    myls/Options/Options.h
    myls/Options/Options.cpp
    myls/Parse/Parse.h
    myls/Parse/Parse.cpp
    myls/Settings/Settings.h
    myls/Settings/Settings.cpp
    myls/SortingOptions/SortingOptions.h
    myls/SortingOptions/SortingOptions.cpp
    myls/main.cpp
)
add_executable(
    mymkdir
    mymkdir/Action/Action.h
    mymkdir/Action/Action.cpp
    mymkdir/Parser/Parser.h
    mymkdir/Parser/Parser.cpp
    mymkdir/Settings/Settings.h
    mymkdir/Settings/Settings.cpp
    mymkdir/Tokens/Tokens.h
    mymkdir/Tokens/Tokens.cpp
    mymkdir/main.cpp
)
add_executable(
    mymv
    mymv/Action/Action.h
    mymv/Action/Action.cpp
    mymv/Parser/Parser.h
    mymv/Parser/Parser.cpp
    mymv/Settings/Settings.h
    mymv/Settings/Settings.cpp
    mymv/Tokens/Tokens.h
    mymv/Tokens/Tokens.cpp
    mymv/main.cpp
)
add_executable(
    myrm
    myrm/Action/Action.h
    myrm/Action/Action.cpp
    myrm/Parser/Parser.h
    myrm/Parser/Parser.cpp
    myrm/Settings/Settings.h
    myrm/Settings/Settings.cpp
    myrm/Tokens/Tokens.h
    myrm/Tokens/Tokens.cpp
    myrm/main.cpp
)
add_executable(
    mycp
    mycp/Action/Action.h
    mycp/Action/Action.cpp
    mycp/Parser/Parser.h
    mycp/Parser/Parser.cpp
    mycp/Settings/Settings.h
    mycp/Settings/Settings.cpp
    mycp/Tokens/Tokens.h
    mycp/Tokens/Tokens.cpp
    mycp/main.cpp
)
add_executable(
    myshell
    myshell/Common/Common.h
    myshell/Common/Common.cpp
    myshell/ExternalCommand/ExternalCommand.h
    myshell/ExternalCommand/ExternalCommand.cpp
    myshell/InternalCommand/InternalCommand.h
    myshell/InternalCommand/InternalCommand.cpp
    myshell/Parse/Parse.h
    myshell/Parse/Parse.cpp
    myshell/Tokens/Tokens.h
    myshell/Tokens/Tokens.cpp
    myshell/Wildcard/Wildcard.h
    myshell/Wildcard/Wildcard.cpp
    myshell/Environ/Environ.h
    myshell/Environ/Environ.cpp
    myshell/main.cpp
)

TARGET_LINK_LIBRARIES( myshell LINK_PUBLIC ${Boost_LIBRARIES} )