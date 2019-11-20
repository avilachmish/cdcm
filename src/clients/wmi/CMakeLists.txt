cmake_minimum_required(VERSION 3.9.0)
project(cdcm)

include(../../Openvas.cmake)
include_directories(${OPENVAS_SMB_INCLUDES})
link_directories(${OPENVAS_SMB_LINKS})

set(SOURCE_FILES wmi_client.cpp)
set(OPENVAS_INCLUDES /var/openvas-smb/wmi /var/openvas-smb/samba/include)

add_library(tw-wmi-client SHARED ${SOURCE_FILES})

target_link_libraries(tw-wmi-client  openvas_wmiclient  tw-singleton-runner)

install(TARGETS tw-wmi-client  LIBRARY DESTINATION lib
        DESTINATION ${lib_dest})

install(TARGETS tw-wmi-client  LIBRARY DESTINATION DESTINATION lib
        DESTINATION ${release_lib_dest})