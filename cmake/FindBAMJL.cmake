

find_path(BAMJL_ROOT_DIR
    NAMES include/BAMJL.h
)

find_path(BAMJL_INCLUDE_DIRS
    NAMES BAMJL.h
    HINTS ${BAMJL_ROOT_DIR}/include
)

find_library(BAMJL_LIBRARY
    NAMES BAMJL
    HINTS ${BAMJL_ROOT_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BAMJL DEFAULT_MSG
    BAMJL_INCLUDE_DIRS
    BAMJL_LIBRARY
)

mark_as_advanced(
    BAMJL_ROOT_DIR
    BAMJL_INCLUDE_DIRS
    BAMJL_LIBRARY
)