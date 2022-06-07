cmake_minimum_required(VERSION 3.16)

project(BAM_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE incs include/*)
file(GLOB_RECURSE srcs src/*)

add_executable(${PROJECT_NAME}
    ${incs}
    ${srcs}
)

include_directories(${PROJECT_NAME}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    BAM_cpp
)