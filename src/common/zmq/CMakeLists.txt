cmake_minimum_required(VERSION 3.9.0)
project(cdcm)
set(SOURCE_FILES zmq_helpers.cpp zmq_message.cpp)
add_library(tw-zmq SHARED ${SOURCE_FILES})

install(TARGETS tw-zmq LIBRARY DESTINATION lib
        DESTINATION ${lib_dest})

install(TARGETS tw-zmq LIBRARY DESTINATION lib
        DESTINATION ${release_lib_dest})