cmake_minimum_required(VERSION 3.9.0)
project(cdcm)

include(../../Openvas.cmake)
include_directories(${OPENVAS_SMB_INCLUDES})
link_directories(${OPENVAS_SMB_LINKS})

set(SOURCE_FILES wmi_client.cpp)
set(OPENVAS_INCLUDES /var/openvas-smb/wmi /var/openvas-smb/samba/include)

add_library(tw_wmi_client SHARED ${SOURCE_FILES})
#add_library(openvas_credentials OBJECT ${OPENVAS_SMB_SRC}/samba/auth/credentials/credentials.c)

target_link_libraries(tw_wmi_client openvas_wmiclient  tw-singleton-runner)
#target_link_libraries(tw_wmi_client   ) #rotem: temporary for cli_credentials
install(TARGETS tw_wmi_client LIBRARY DESTINATION lib
        DESTINATION ${lib_dest})

install(TARGETS tw_wmi_client LIBRARY DESTINATION DESTINATION lib
        DESTINATION ${release_lib_dest})