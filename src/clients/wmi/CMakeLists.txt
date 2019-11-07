cmake_minimum_required(VERSION 3.9.0)
project(cdcm)

include_directories(${SMB_INCLUDES} /usr/include)
link_directories(${SMB_LINKS})
link_directories(/usr/local/lib /usr/lib64)
#add_library( smbclient-raw SHARED IMPORTED )
#set_property( TARGET smbclient-raw PROPERTY IMPORTED_LOCATION ${SAMBA_SRC}/bin/shared/private/libsmbclient-raw-samba4.so )
#
#add_library( smbclient SHARED IMPORTED )
#set_property( TARGET smbclient PROPERTY IMPORTED_LOCATION ${SAMBA_SRC}/bin/default/source3/libsmb/libsmbclient.so )

set(SOURCE_FILES wmi_client.cpp)
#set (OPENVAS_LIB_PATH )
set(OPENVAS_INCLUDES /var/openvas-smb/wmi /var/openvas-smb/samba/include)

add_library(tw_wmi_client SHARED ${SOURCE_FILES})
target_link_libraries(openvas_wmiclient tw-singleton-runner)

install(TARGETS tw-smb-client LIBRARY DESTINATION lib
        DESTINATION ${lib_dest})

install(TARGETS tw-smb-client LIBRARY DESTINATION DESTINATION lib
        DESTINATION ${release_lib_dest})