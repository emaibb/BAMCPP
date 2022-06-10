

find_path(BAMJL_ROOT_DIR
    NAMES include/BAMJL.h
)

find_path(BAMJL_INCLUDE_DIR
    NAMES BAMJL.h
    HINTS ${BAM_ROOT_DIR}/include
)

find_library(BAMJL_LIBRARY
    NAMES BAMJL
    HINTS ${BAM_ROOT_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BAM DEFAULT_MSG
    BAMJL_INCLUDE_DIR
    BAMJL_LIBRARY
)

mark_as_advanced(
    BAMJL_ROOT_DIR
    BAMJL_INCLUDE_DIR
    BAMJL_LIBRARY
)