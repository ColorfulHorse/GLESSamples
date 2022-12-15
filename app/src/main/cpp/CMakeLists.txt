# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

set(CXX_STANDARD 11)

project("glsample")

set(LIBS_DIR "${CMAKE_SOURCE_DIR}/libs")

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_LIST "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE INCLUDE_LIST "${PROJECT_SOURCE_DIR}/include/*.h" "${PROJECT_SOURCE_DIR}/include/*.hpp")

add_library(
        glsample
        SHARED
        ${SRC_LIST}
)

find_library(
        log-lib
        log)

add_library(freetype SHARED IMPORTED)
set_target_properties(freetype
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/${ANDROID_ABI}/libfreetype.so)

target_link_libraries(
        glsample
        ${log-lib}
        -lGLESv3
        freetype
        -landroid
)