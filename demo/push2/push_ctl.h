#pragma once

#include <memory>
#include "yangutil/yangavinfotype.h"
#include <yangpush/YangPushFactory.h>


class PushCtl : public YangSysMessageI, public YangSysMessageHandleI {

public:
    PushCtl();

    ~PushCtl();

    void start();

public:
    void success() override;

    void failure(int32_t errcode) override;

    void receiveSysMessage(YangSysMessage *mss, int32_t err) override;


private:
    void init();


private:
    std::shared_ptr<YangContext> m_context;
    int m_videoType;
    bool m_isStartpush;
    bool m_hasAudio;

    YangVideoInfo m_screenInfo;
    YangVideoInfo m_outInfo;


    string m_url;

    YangSysMessageHandle *m_message;

    YangPushFactory m_pf;


};
