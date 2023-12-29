
#include "push_ctl.h"
#include <yangutil/sys/YangSocket.h>
#include <yangutil/sys/YangString.h>
#include <yangpush/YangPushCommon.h>

PushCtl::PushCtl() {
    m_context = std::make_shared<YangContext>();
    m_context->init("yang_config.ini");
    m_context->avinfo.video.videoEncoderFormat = YangI420;
#if Yang_Enable_Openh264
    m_context->avinfo.enc.createMeta = 0;
#else
    m_context->avinfo.enc.createMeta = 1;
#endif
#if Yang_Enable_GPU_Encoding
    //using gpu encode
    m_context->avinfo.video.videoEncHwType=YangV_Hw_Nvdia;//YangV_Hw_Intel,  YangV_Hw_Nvdia,
    m_context->avinfo.video.videoEncoderFormat=YangI420;//YangI420;//YangArgb;
    m_context->avinfo.enc.createMeta=0;
#endif

    init();
    yang_setLogLevel(m_context->avinfo.sys.logLevel);
    yang_setLogFile(m_context->avinfo.sys.enableLogFile);

    m_context->avinfo.sys.mediaServer = Yang_Server_Srs;//Yang_Server_Srs/Yang_Server_Zlm/Yang_Server_Whip_Whep
    m_context->avinfo.rtc.rtcLocalPort = 10000 + yang_random() % 15000;
    //m_showev->event=1;

    m_videoType = Yang_VideoSrc_Camera;//Yang_VideoSrc_Camera/Yang_VideoSrc_Screen;

    m_isStartpush = false;


    char s[128] = {0};
    memset(m_context->avinfo.rtc.localIp, 0, sizeof(m_context->avinfo.rtc.localIp));
    yang_getLocalInfo(m_context->avinfo.sys.familyType, m_context->avinfo.rtc.localIp);
    sprintf(s, "http://192.168.3.247:9060/index/api/whip?app=live&stream=test");

    m_hasAudio = false;

    m_context->avinfo.rtc.enableDatachannel = yangfalse;

    //using h264 h265
    m_context->avinfo.video.videoEncoderType = Yang_VED_264;//Yang_VED_265;
    if (m_context->avinfo.video.videoEncoderType == Yang_VED_265) {
        m_context->avinfo.enc.createMeta = 1;
        m_context->avinfo.rtc.enableDatachannel = yangfalse;
    }
    m_context->avinfo.rtc.iceCandidateType = YangIceHost;

    //srs do not use audio fec
    m_context->avinfo.audio.enableAudioFec = yangfalse;

#if Yang_OS_APPLE
    m_context->avinfo.video.videoCaptureFormat=YangNv12;
#endif


    m_message = m_pf.createPushMessageHandle(m_hasAudio, false, m_videoType, &m_screenInfo, &m_outInfo,
                                             m_context.get(), this, this);


    m_message->start();

    if (m_videoType == Yang_VideoSrc_Screen) {
        yang_post_message(YangM_Push_StartScreenCapture, 0, NULL);
    } else if (m_videoType == Yang_VideoSrc_Camera) {
        yang_post_message(YangM_Push_StartVideoCapture, 0, NULL);
    } else if (m_videoType == Yang_VideoSrc_OutInterface) {
        yang_post_message(YangM_Push_StartOutCapture, 0, NULL);
    }
}

PushCtl::~PushCtl() {
    if (m_context) {
//        m_rt->stopAll();
//        m_rt = NULL;
        yang_delete(m_message);
        m_context;
    }
}


void PushCtl::start() {
    // ui->m_b_record->setEnabled(false);
    if (!m_isStartpush) {
        yang_info("start================");
        m_isStartpush = !m_isStartpush;
        m_url = "http://192.168.3.247:9060/index/api/whip?app=live&stream=test";
        yang_post_message(YangM_Push_Connect_Whip, 0,
                          NULL, (void *) m_url.c_str());
        // yang_post_message(YangM_Push_Connect,0,NULL,(void*)m_url.c_str());
    } else {
        yang_info("stop");
        yang_post_message(YangM_Push_Disconnect, 0, NULL);
        m_isStartpush = !m_isStartpush;
    }

}


void PushCtl::success() {

}

void PushCtl::failure(int32_t errcode) {

}

void PushCtl::receiveSysMessage(YangSysMessage *mss, int32_t err) {
    switch (mss->messageId) {
        case YangM_Push_Connect: {
            if (err) {
                yang_info("start push error:%d", err);
                m_isStartpush = !m_isStartpush;
            }
        }
            break;
        case YangM_Push_Disconnect:
            break;
        case YangM_Push_StartScreenCapture:
            yang_error("start screen capture error:%d", err);
            break;
        case YangM_Push_StartVideoCapture: {
            yang_error("start video capture error:%d", err);
            break;
        }
        case YangM_Push_StartOutCapture: {
            yang_error("start out capture error:%d", err);
            break;
        }
        case YangM_Push_SwitchToCamera:
            break;
        case YangM_Push_SwitchToScreen:
            break;
    }
}

void PushCtl::init() {
    m_context->avinfo.audio.enableMono = yangfalse;
    m_context->avinfo.audio.sample = 48000;
    m_context->avinfo.audio.channel = 2;
    m_context->avinfo.audio.enableAec = yangfalse;
    m_context->avinfo.audio.audioCacheNum = 8;
    m_context->avinfo.audio.audioCacheSize = 8;
    m_context->avinfo.audio.audioPlayCacheNum = 8;

    m_context->avinfo.video.videoCacheNum = 10;
    m_context->avinfo.video.evideoCacheNum = 10;
    m_context->avinfo.video.videoPlayCacheNum = 10;

    m_context->avinfo.audio.audioEncoderType = Yang_AED_OPUS;
    m_context->avinfo.rtc.rtcLocalPort = 17000;
    m_context->avinfo.enc.enc_threads = 4;

}


