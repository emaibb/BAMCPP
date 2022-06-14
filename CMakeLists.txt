cmake_minimum_required(VERSION 3.16)

project(BAM_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE incs include/*)
file(GLOB_RECURSE srcs src/*)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(BAMJL REQUIRED)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BAMJL_ROOT_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BAMJL_ROOT_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BAMJL_ROOT_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BAMJL_ROOT_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BAMJL_ROOT_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BAMJL_ROOT_DIR}/bin )

add_library(${PROJECT_NAME} SHARED
    ${incs}
    ${srcs}
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${BAMJL_ROOT_DIR}/include
)

include_directories(${PROJECT_NAME}
    ${Julia_INCLUDE_DIRS}
    ${BAMJL_INCLUDE_DIRS}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(Julia REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${Julia_LIBRARY}
    ${BAMJL_LIBRARY}
)

option(BUILD_TEST "Enables build of TEST" ON)
if(${BUILD_TEST})
    add_subdirectory(test)
endif()