cmake_minimum_required(VERSION 3.9.0)
project(cdcm)

include(../../Openvas.cmake)

link_directories(${OPENVAS_SMB_LINKS})
include_directories(${OPENVAS_SMB_INCLUDES})
include_directories(../../src)

set(SOURCE_FILES wql_action.cpp)
add_library(tw-wmi-actions OBJECT ${SOURCE_FILES})
target_link_libraries(tw-wmi-actions tw-wmi-client tw-common)
install(TARGETS tw-wmi-actions LIBRARY DESTINATION lib
        DESTINATION ${lib_dest})

install(TARGETS tw-wmi-actions  LIBRARY  DESTINATION lib
        DESTINATION ${release_lib_dest})