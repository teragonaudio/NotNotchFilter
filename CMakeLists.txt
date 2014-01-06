project(NotNotchFilter)
cmake_minimum_required(VERSION 2.6)

# set the output destination
set(NOTNOTCHFILTER_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Builds/Linux/build/NotNotchFilter.so)

# create a custom target that will run each time you type make 
add_custom_target(build_NotNotchFilter ALL 
                  COMMAND ${CMAKE_MAKE_PROGRAM}
                  WORKING_DIRECTORY
                  ${CMAKE_CURRENT_SOURCE_DIR}/Builds/Linux
                  COMMENT "Building Introjucer generated Makefile")

# now create an imported static target
add_library(NotNotchFilter SHARED IMPORTED)
set_target_properties(NotNotchFilter PROPERTIES PREFIX "")
set_property(TARGET NotNotchFilter APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(NotNotchFilter PROPERTIES IMPORTED_LOCATION_NOCONFIG "${NOTNOTCHFILTER_LIBRARY}")

add_dependencies(NotNotchFilter build_NotNotchFilter)
