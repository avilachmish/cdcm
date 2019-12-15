cmake_minimum_required(VERSION 3.9.0)
project(cdcm)
include(../Openvas.cmake)

set(SOURCE_FILES wmi_client.cpp)
add_library(tw-wmi-client SHARED ${SOURCE_FILES})
target_link_libraries(tw-wmi-client  openvas_wmiclient tw-singleton-runner ${OPENVAS_SMB_LIBS})

add_subdirectory(actions)

install(TARGETS tw-wmi-client LIBRARY DESTINATION lib
        DESTINATION ${lib_dest})

install(TARGETS tw-wmi-client LIBRARY DESTINATION DESTINATION lib
        DESTINATION ${release_lib_dest})