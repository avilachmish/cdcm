cmake_minimum_required(VERSION 3.9.0)
project(cdcm)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include_directories(. /usr/local/include/openwsman)

set(SOURCE_FILES winrm_client.cpp epr.cpp options.cpp filter.cpp)
add_library(tw-winrm-client STATIC ${SOURCE_FILES})

target_link_libraries(tw-winrm-client wsman wsman_client wsman_curl_client_transport xml2 curl ssl crypto pthread dl)
