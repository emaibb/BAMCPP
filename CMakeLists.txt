cmake_minimum_required(VERSION 3.16)

project(BAM_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE incs include/*)
file(GLOB_RECURSE srcs src/*)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Julia REQUIRED)
find_package(BAM REQUIRED)

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BAM_LIBRARY}/../../lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BAM_LIBRARY}/../../bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BAM_LIBRARY}/../../lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BAM_LIBRARY}/../../bin )

add_library(${PROJECT_NAME} SHARED
    ${incs}
    ${srcs}
)

include_directories(${PROJECT_NAME}
    ${Julia_INCLUDE_DIRS}
    ${BAM_INCLUDE_DIR}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${Julia_LIBRARY}
    ${BAM_LIBRARY}
)

option(BUILD_TEST "Enables build of TEST" ON)
if(${BUILD_TEST})
    add_subdirectory(test)
endif()