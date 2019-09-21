file(GLOB RB_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.rb *.xml exe_test)
message(STATUS ${RB_SRCS})
add_custom_target(
        functional_tests
        COMMAND ${CMAKE_COMMAND} -E copy
        ${RB_SRCS}
        ${CMAKE_CURRENT_BINARY_DIR}/)
