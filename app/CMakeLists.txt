cmake_minimum_required(VERSION 3.4.1)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../distribution)

# set add lib
add_library(lib_add SHARED IMPORTED)
set_target_properties(lib_add PROPERTIES IMPORTED_LOCATION
                      ${distribution_DIR}/libs/${ANDROID_ABI}/libadd.so)

# set opencv libs
add_library(lib_opencv_java3 SHARED IMPORTED)
set_target_properties(lib_opencv_java3 PROPERTIES
                      IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libopencv_java3.so)
include_directories(lib_opencv_java3
                    ${distribution_DIR}/include)


add_library(native-math SHARED
            src/main/cpp/native-math.cpp)
set_target_properties(native-math PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY ${distribution_DIR}/libs/${ANDROID_ABI})
target_link_libraries(native-math
                      android
                      lib_add
                      log)

add_library(native-opencv SHARED
            src/main/cpp/native-opencv.cpp)
set_target_properties(native-opencv PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY ${distribution_DIR}/libs/${ANDROID_ABI})
target_link_libraries(native-opencv
                      android
                      lib_opencv_java3
                      log)