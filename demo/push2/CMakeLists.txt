project(push2)

# `std::make_unique`要求最低C++14
set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(push2
        main.cpp
        push_ctl.cpp
        yangpush/YangPushCapture.cpp
        yangpush/YangPushEncoder.cpp
        yangpush/YangPushFactory.cpp
        yangpush/YangPushHandleImpl.cpp
        yangpush/YangPushMessageHandle.cpp
        yangpush/YangPushPublish.cpp
        yangpush/YangRtcPublish.cpp
        yangpush/YangSendVideoImpl.cpp
)

message("MK_LINK_LIBRARIES:${MK_LINK_LIBRARIES}")
# speexdsp opus asound ssl srtp2 libyuv.a libopenh264.a usrsctp crypto
target_link_libraries(push2
        ${MK_LINK_LIBRARIES}
        asound libyuv.a usrsctp
)


