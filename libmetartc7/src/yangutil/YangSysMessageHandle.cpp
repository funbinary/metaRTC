﻿//
// Copyright (c) 2019-2022 yanggaofeng
//


#include <yangutil/sys/YangSysMessageHandle.h>

#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangThread.h>
#include <yangutil/yangavinfotype.h>
void yang_post_message(int32_t st, int32_t uid, YangSysMessageI *mhandle,void* user) {
	if (YangSysMessageHandle::m_instance){
        yang_info("putmessage:%d", st);
        YangSysMessageHandle::m_instance->putMessage(mhandle, st, uid, 0,user);
    }

}
void yang_post_state_message(int32_t st, int32_t uid, int32_t handleState,YangSysMessageI *mhandle) {
	if (YangSysMessageHandle::m_instance)
		YangSysMessageHandle::m_instance->putMessage(mhandle, st, uid,
				handleState);
}
void yang_post_userMessage(int32_t st, int32_t uid,YangSysMessageI *mhandle,void* user){
    if (YangSysMessageHandle::m_instance)
        YangSysMessageHandle::m_instance->putMessage(mhandle, st, uid, 0);
}
YangSysMessageHandle::YangSysMessageHandle() {
	m_loop = yangfalse;
	m_isStart = yangfalse;
	yang_thread_mutex_init(&m_mutex, NULL);
	yang_thread_mutex_init(&m_lock, NULL);
	yang_thread_cond_init(&m_cond_mess, NULL);
	m_waitState = yangfalse;
	m_receive=NULL;
	if (m_instance == NULL)
		m_instance = this;
}

YangSysMessageHandle::~YangSysMessageHandle() {

	if (m_isStart) {
		stop();
		while (m_isStart) {
			yang_usleep(1000);
		}
	}
	m_instance = NULL;
	yang_thread_mutex_destroy(&m_mutex);
	yang_thread_mutex_destroy(&m_lock);
	yang_thread_cond_destroy(&m_cond_mess);

}
YangSysMessageHandle *YangSysMessageHandle::m_instance = NULL;
void YangSysMessageHandle::run() {
	m_isStart = yangtrue;
    startLoop();
	m_isStart = yangfalse;
}
void YangSysMessageHandle::stop() {
	stopLoop();

}
void YangSysMessageHandle::stopLoop() {
	m_loop = yangfalse;
	//yang_thread_mutex_unlock(&m_lock);
	if(m_waitState){
		yang_thread_mutex_lock(&m_lock);
		yang_thread_cond_signal(&m_cond_mess);
		yang_thread_mutex_unlock(&m_lock);

	}
}

void YangSysMessageHandle::putMessage(YangSysMessageI *handle, int32_t pst,
        int32_t puid, int32_t handleState,void* user) {
	if(!m_loop) return;

	YangSysMessage* mes=new YangSysMessage();

	mes->uid = puid;
	mes->messageId = pst;
	mes->handleState = handleState;
	mes->handle = handle;
    mes->user=user;
    yang_thread_mutex_lock(&m_mutex);
	m_sysMessages.push_back(mes);
	yang_thread_mutex_unlock(&m_mutex);
	mes=NULL;
    yang_info("state: %d",m_waitState);
	if(m_waitState){
		yang_thread_mutex_lock(&m_lock);
		yang_thread_cond_signal(&m_cond_mess);
		yang_thread_mutex_unlock(&m_lock);
	}
}

void YangSysMessageHandle::startLoop() {
   m_loop = 1;
   yang_thread_mutex_unlock(&m_lock);
    yang_thread_mutex_lock(&m_lock);
    yang_info("startloop");

    while (m_loop) {

        m_waitState=yangtrue;
        yang_info("before cond");
        yang_thread_cond_wait(&m_cond_mess, &m_lock);
        m_waitState=yangfalse;
        yang_info("message size:%d",m_sysMessages.size());

        while (m_sysMessages.size()>0) {
            auto m = m_sysMessages.front();
            yang_info("push message:%d",m->messageId);
            handleMessage(m);
            m_sysMessages.front()->handle = NULL;
            delete m_sysMessages.front();
            m_sysMessages.front() = NULL;
            yang_thread_mutex_lock(&m_mutex);
            m_sysMessages.erase(m_sysMessages.begin());
            yang_thread_mutex_unlock(&m_mutex);
        }
    }
    yang_thread_mutex_unlock(&m_lock);
}

