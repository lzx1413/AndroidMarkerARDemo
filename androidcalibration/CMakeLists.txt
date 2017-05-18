cmake_minimum_required(VERSION 3.4.1)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../distribution)

# set add lib
include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()

add_library(
    caffe2
    STATIC
    IMPORTED
    )
set_target_properties(
    caffe2
    PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/libs/${ANDROID_ABI}/libcaffe2.a
    )

    add_library(
        glog
        SHARED
        IMPORTED
        )
    set_target_properties(
        glog
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libs/${ANDROID_ABI}/libglog.so
        )
    add_library(
        protobuf
        SHARED
        IMPORTED
        )
    set_target_properties(
        protobuf
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libs/${ANDROID_ABI}/libprotobuf.a
        )

    add_library(
        NNPACK
        STATIC
        IMPORTED
        )
    set_target_properties(
        NNPACK
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/libs/${ANDROID_ABI}/libNNPACK.a
        )
add_library(lib_add SHARED IMPORTED)
set_target_properties(lib_add PROPERTIES IMPORTED_LOCATION
                      ${distribution_DIR}/libs/${ANDROID_ABI}/libadd.so)

# set opencv libs
add_library(lib_opencv_java3 SHARED IMPORTED)

set_target_properties(lib_opencv_java3 PROPERTIES
                      IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libopencv_java3.so)
include_directories(${distribution_DIR}/include)
include_directories(src/main/cpp/)
add_library(native-opencv SHARED
            src/main/cpp/MarkerDetector.cpp
            src/main/cpp/native-opencv.cpp
            src/main/cpp/GeometryTypes.cpp
            src/main/cpp/Marker.cpp
            src/main/cpp/TinyLA.cpp
            src/main/cpp/CameraCalibration.cpp
            src/main/cpp/ARDisplay.cpp
            )
set_target_properties(native-opencv PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY ${distribution_DIR}/libs/${ANDROID_ABI})
target_link_libraries(native-opencv
                      android
                      caffe2
                      lib_opencv_java3
                      log
                      -Wl,--no-whole-archive
                      NNPACK
                      glog
                      protobuf
                      cpufeatures

                      )