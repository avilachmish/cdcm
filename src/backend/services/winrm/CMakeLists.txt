#
# CMakeLists.txt for openwsman/src/cpp
#
include_directories(/usr/local/include/openwsman/cpp /usr/local/include/openwsman)
add_executable(my_client my_client.cpp)
target_link_libraries(my_client wsman_clientpp wsman  wsman_client wsman_curl_client_transport xml2 curl ssl crypto pthread dl)
add_subdirectory(actions)

install(TARGETS my_client RUNTIME DESTINATION bin
        DESTINATION ${bin_dest})
install(TARGETS my_client
        RUNTIME DESTINATION bin
        DESTINATION ${release_bin_dest})