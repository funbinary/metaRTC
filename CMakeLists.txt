cmake_minimum_required(VERSION 3.18.0)
project(metartc)


option(ENABLE_OPENSSL "Enable OpenSSL" ON)

set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/release)
set(LIBRARY_OUTPUT_PATH ${OUTPUT_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR}/bin)

# 方便修改全局变量
function(update_cached name value)
    set("${name}" "${value}" CACHE INTERNAL "*** Internal ***" FORCE)
endfunction()

function(update_cached_list name)
    set(_tmp_list "${${name}}")
    list(APPEND _tmp_list "${ARGN}")
    list(REMOVE_DUPLICATES _tmp_list)
    update_cached(${name} "${_tmp_list}")
endfunction()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

update_cached(MK_LINK_LIBRARIES "")

# ssl库
set(OPENSSL_USE_STATIC_LIBS TRUE) # 使用静态库
find_package(OpenSSL QUIET)
if (OPENSSL_FOUND)
    # https://cmake.org/cmake/help/latest/module/FindOpenSSL.html
    message(STATUS "found library: ${OPENSSL_LIBRARIES}, ENABLE_OPENSSL defined")
    include_directories(${OPENSSL_INCLUDE_DIR})
    update_cached_list(MK_COMPILE_DEFINITIONS ENABLE_OPENSSL)
    update_cached_list(MK_LINK_LIBRARIES ${OPENSSL_LIBRARIES})
    if (OPENSSL_USE_STATIC_LIBS)
        update_cached_list(MK_LINK_LIBRARIES ${CMAKE_DL_LIBS})
    endif ()
else ()
    message(FATAL_ERROR "openssl 未找到")
endif ()


# thread
find_package(Threads REQUIRED)
update_cached_list(MK_LINK_LIBRARIES Threads::Threads)

# speedxdsp
find_package(Libspeexdsp REQUIRED)
if (LIBSPEEXDSP_FOUND)
    message(STATUS "found library: ${LIBSPEEXDSP_LIBRARIES}")
    include_directories(${LIBSPEEXDSP_INCLUDE_DIRS})
    update_cached_list(MK_LINK_LIBRARIES ${LIBSPEEXDSP_LIBRARIES})
else ()
    message(FATAL_ERROR "speexdsp未找到")
endif ()

# srtp
find_package(SRTP QUIET)
if (SRTP_FOUND)
    message(STATUS "found library: ${SRTP_LIBRARIES}")
    include_directories(${SRTP_INCLUDE_DIRS})
    update_cached_list(MK_LINK_LIBRARIES ${SRTP_LIBRARIES})
else ()
    message(FATAL_ERROR "srtp未找到")
endif ()

# opus
find_package(Opus QUIET)
if (OPUS_FOUND)
    message(STATUS "found library: ${OPUS_STATIC_LIBRARY}")
    include_directories(${OPUS_INCLUDE_DIRS})
    update_cached_list(MK_LINK_LIBRARIES ${OPUS_STATIC_LIBRARY})
else ()
    message(FATAL_ERROR "opus未找到")
endif ()

# openh264
find_package(Openh264 QUIET)
if (Openh264_FOUND)
    message(STATUS "found library: ${Openh264_STATIC_LIBRARY}")
    include_directories(${Openh264_INCLUDE_DIR})
    update_cached_list(MK_LINK_LIBRARIES ${Openh264_STATIC_LIBRARY})
else ()
    message(FATAL_ERROR "openh264未找到")
endif ()


include_directories(libmetartc7/src)
include_directories(thirdparty/include)
include_directories(include)
add_subdirectory(libmetartc7)
add_subdirectory(libmetartccore7)
add_subdirectory(demo)
