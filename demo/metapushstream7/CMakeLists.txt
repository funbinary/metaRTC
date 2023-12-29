
project(metapushstream7)

# `std::make_unique`要求最低C++14
set(CMAKE_CXX_STANDARD 14)

#add_compile_definitions(QT_DEPRECATED_WARNINGS)

#add_executable(metapushstream7)

message(${MK_LINK_LIBRARIES})
##


######################################## 添加qt5库 ########################################
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
if (Qt5_FOUND)
    set(qt5_libraries)
    list(APPEND qt5_libraries Qt5::Core Qt5::Gui Qt5::Widgets)
    message(STATUS "Qt5 libraries: ${qt5_libraries}")
else ()
    message(WARNING "Qt5 not found")
    return()
endif ()
# 设置自动生成moc/rcc/uic文件(一定要设置)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# Generate code from the UI file
qt5_wrap_ui(UI_FILES
        recordmainwindow.ui
        yangjanus.ui)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/video)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/yangpush)

add_executable(metapushstream7
        main.cpp
        ${UI_FILES}
        recordmainwindow.cpp
        video/YangPlayWidget.cpp
        video/YangYuvPlayWidget.cpp
        video/yangrecordthread.cpp
        video/yangrecordvideowin.cpp
        yangjanus.cpp
        yangpush/YangPushCapture.cpp
        yangpush/YangPushEncoder.cpp
        yangpush/YangPushFactory.cpp
        yangpush/YangPushHandleImpl.cpp
        yangpush/YangPushMessageHandle.cpp
        yangpush/YangPushPublish.cpp
        yangpush/YangRtcPublish.cpp
        yangpush/YangSendVideoImpl.cpp

)
message(${MK_LINK_LIBRARIES})
target_link_libraries(metapushstream7
        ${MK_LINK_LIBRARIES} asound libyuv.a usrsctp
        ${qt5_libraries})


