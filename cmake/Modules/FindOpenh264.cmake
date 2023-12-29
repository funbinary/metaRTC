find_path(Openh264_INCLUDE_DIR NAMES wels/codec_api.h)

find_library(Openh264_LIBRARY NAMES openh264)

find_library(Openh264_STATIC_LIBRARY NAMES libopenh264.a)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Openh264
        DEFAULT_MSG
        Openh264_INCLUDE_DIR
        Openh264_LIBRARY
        Openh264_STATIC_LIBRARY)

if (Openh264_FOUND)
    set(Openh264_INCLUDE_DIRS ${Openh264_INCLUDE_DIR})

    if (Openh264_LIBRARY)
        set(Openh264_LIBRARIES ${Openh264_LIBRARY})
    endif ()

    if (Openh264_STATIC_LIBRARY)
        set(Openh264_STATIC_LIBRARIES ${Openh264_STATIC_LIBRARY})
    endif ()
endif ()

mark_as_advanced(
        Openh264_INCLUDE_DIR
        Openh264_LIBRARY
        Openh264_STATIC_LIBRARY
)