cmake_minimum_required(VERSION 3.9.0)
project(cdcm)
#rotem TODO place inside folders
include(samba.cmake)
add_subdirectory(registry)
add_subdirectory(wmi)
add_subdirectory(smb)
