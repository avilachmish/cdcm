cmake_minimum_required(VERSION 3.9.0)
project(cdcm)
include(../samba.cmake)

enable_cxx_compiler_flag_if_supported("-Wno-builtin-macro-redefined")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(SOURCE_FILES registry_client.cpp)
add_definitions(-D_SPLINT_)
add_library(tw-registry-client STATIC ${SOURCE_FILES})
target_link_libraries(tw-registry-client s4_events tevent samba-errors talloc samba-util samba-hostconfig registry-samba4 registry_common)
add_library(registry_common OBJECT ${SAMBA_SRC}/source4/lib/registry/tools/common.c)
add_library(s4_events OBJECT ${SAMBA_SRC}/source4/lib/events/tevent_s4.c)

add_subdirectory(actions)
install(TARGETS tw-registry-client s4_events registry_common ARCHIVE DESTINATION lib
        DESTINATION ${lib_dest})
install(TARGETS tw-registry-client s4_events registry_common ARCHIVE DESTINATION lib
        DESTINATION ${release_lib_dest})

install(FILES registry.json  DESTINATION ${release_lib_dest}/plugins/)

#configure_file(registry.json ${release_lib_dest}/plugins/registry.json COPYONLY)
