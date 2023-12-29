# FindOpus.cmake

# 查找Opus库
find_path(OPUS_INCLUDE_DIR NAMES opus/opus.h PATH_SUFFIXES include)


# 查找动态库
find_library(OPUS_LIBRARY NAMES opus PATH_SUFFIXES
        lib${_lib_suffix}
        lib
        libs${_lib_suffix}
        libs
        bin${_lib_suffix}
        bin
        ../lib${_lib_suffix}
        ../lib
        ../libs${_lib_suffix}
        ../libs
        ../bin${_lib_suffix}
        ../bin)

# 查找静态库
find_library(OPUS_STATIC_LIBRARY NAMES libopus.a PATH_SUFFIXES
        lib${_lib_suffix}
        lib
        libs${_lib_suffix}
        libs
        bin${_lib_suffix}
        bin
        ../lib${_lib_suffix}
        ../lib
        ../libs${_lib_suffix}
        ../libs
        ../bin${_lib_suffix}
        ../bin)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Opus DEFAULT_MSG OPUS_INCLUDE_DIR OPUS_LIBRARY OPUS_STATIC_LIBRARY)

if (OPUS_FOUND)
    set(OPUS_INCLUDE_DIRS ${OPUS_INCLUDE_DIR})
    if (OPUS_LIBRARY)
        set(OPUS_LIBRARIES ${OPUS_LIBRARY})
    endif ()
    if (OPUS_STATIC_LIBRARY)
        set(OPUS_STATIC_LIBRARIES ${OPUS_STATIC_LIBRARY})
    endif ()
endif ()

mark_as_advanced(OPUS_INCLUDE_DIR OPUS_LIBRARY OPUS_STATIC_LIBRARY)