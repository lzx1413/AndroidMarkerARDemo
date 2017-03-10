cmake_minimum_required(VERSION 3.4.1)


add_library(add SHARED
            src/main/cpp/add.cpp)

set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../distribution)

set_target_properties(add PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY ${distribution_DIR}/libs/${ANDROID_ABI})
target_include_directories(add
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)