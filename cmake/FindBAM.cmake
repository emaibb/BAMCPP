

find_path(BAM_ROOT_DIR
    NAMES include/BAM.h
)

find_path(BAM_INCLUDE_DIR
    NAMES BAM.h
    HINTS ${BAM_ROOT_DIR}/include
)

find_library(BAM_LIBRARY
    NAMES BAMJL
    HINTS ${BAM_ROOT_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BAM DEFAULT_MSG
    BAM_INCLUDE_DIR
    BAM_LIBRARY
)

mark_as_advanced(
    BAM_ROOT_DIR
    BAM_INCLUDE_DIR
    BAM_LIBRARY
)