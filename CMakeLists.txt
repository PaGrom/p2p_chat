cmake_minimum_required(VERSION 2.8)

project(p2p_chat)

set(SOURCE_EXE main.cpp)

set(WINDOW Window.cpp)

set(SERVER Server.cpp)

set(CLIENT Client.cpp)

set(FUNCS funcs.cpp)

add_library(window STATIC ${WINDOW})

add_library(server STATIC ${SERVER})

add_library(client STATIC ${CLIENT})

add_library(funcs STATIC ${FUNCS})

add_executable(p2p_chat ${SOURCE_EXE})

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)

find_package(Threads)

target_link_libraries(p2p_chat window server client funcs ${CMAKE_THREAD_LIBS_INIT} -lncurses)