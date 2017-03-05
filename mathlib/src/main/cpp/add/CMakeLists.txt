cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

add_library(add SHARED add.cpp)

set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../distribution)

set_target_properties(add
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY
                      ${distribution_DIR}/mathlib/add/lib/${ANDROID_ABI})

add_custom_command(TARGET add POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E
                   copy ${CMAKE_CURRENT_SOURCE_DIR}/add.h
                   ${distribution_DIR}/mathlib/add/include/add.h
#                   COMMAND ${CMAKE_COMMAND} -E
#                   copy ${distribution_DIR}/mathlib/add/lib/${ANDROID_ABI}/libadd.so
#                   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libadd.so
                   COMMAND ${CMAKE_COMMAND} -E
                   echo "output libadd.so to ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
                   COMMENT "Copying mathlib/add to output directory")