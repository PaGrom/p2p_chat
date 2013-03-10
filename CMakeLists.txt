cmake_minimum_required(VERSION 2.8)

project(p2p_chat)

set(SOURCE_EXE main.cpp)

set(SERVER Server.cpp)

set(CLIENT Client.cpp)

set(FUNCS funcs.cpp)

add_library(server STATIC ${SERVER})

add_library(client STATIC ${CLIENT})

add_library(funcs STATIC ${FUNCS})

add_executable(p2p_chat ${SOURCE_EXE})

find_package(Threads)

target_link_libraries(p2p_chat server client funcs ${CMAKE_THREAD_LIBS_INIT})