cmake_minimum_required(VERSION 3.9.0)
project(cdcm)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include_directories(. /usr/local/include/openwsman)

add_subdirectory(client)
add_subdirectory(actions)

add_executable(my_client my_client.cpp)
target_link_libraries(my_client tw-winrm-client)

install(TARGETS my_client RUNTIME DESTINATION bin
        DESTINATION ${bin_dest})
install(TARGETS my_client
        RUNTIME DESTINATION bin
        DESTINATION ${release_bin_dest})