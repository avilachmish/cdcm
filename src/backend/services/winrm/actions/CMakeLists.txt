cmake_minimum_required(VERSION 3.9.0)
project(cdcm)

set(LD_ACTION_LIBS -s wsman_clientpp wsman wsman_client wsman_curl_client_transport xml2 curl ssl crypto pthread dl)
set(ACTION_NAMES enumerate)
#get
foreach (X IN ITEMS ${ACTION_NAMES})
    message(STATUS "Building ${X} action")
    add_library(winrm_${X} SHARED ${X}.cpp)
    target_link_libraries(winrm_${X} ${LD_ACTION_LIBS})
endforeach ()

list(TRANSFORM ACTION_NAMES PREPEND "winrm_" OUTPUT_VARIABLE REGISTRY_ACTIONS_LIST)
install(TARGETS ${REGISTRY_ACTIONS_LIST} DESTINATION ${lib_dest}/plugins)

install(TARGETS ${REGISTRY_ACTIONS_LIST} DESTINATION ${release_lib_dest}/plugins)
