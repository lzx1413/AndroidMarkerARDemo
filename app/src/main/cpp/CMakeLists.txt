cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../distribution)

# set the math lib
set(mathlibs_DIR ${distribution_DIR}/mathlib)

add_library(lib_math_add SHARED IMPORTED)
set_target_properties(lib_math_add PROPERTIES IMPORTED_LOCATION
                      ${mathlibs_DIR}/add/lib/${ANDROID_ABI}/libadd.so)

# set opencv libs
set(opencvlibs_DIR ${distribution_DIR}/opencv/libs)

add_library(libopencv_java3 SHARED IMPORTED)
set_target_properties(libopencv_java3 PROPERTIES
                      IMPORTED_LOCATION ${opencvlibs_DIR}/${ANDROID_ABI}/libopencv_java3.so)


# set other things
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

find_library(log-lib log)

add_library(native-math SHARED
            native-math.cpp)
target_include_directories(native-math PUBLIC
                           ${distribution_DIR}/mathlib/add/include)
target_link_libraries(native-math
                      android
                      lib_math_add
                      ${log-lib})

add_library(native-opencv SHARED
            native-opencv.cpp)
target_include_directories(native-opencv PUBLIC
                           ${distribution_DIR}/opencv/include)
target_link_libraries(native-opencv
                      android
                      libopencv_java3
                      ${log-lib})