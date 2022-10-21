//
// Created by admin on 2022/10/8.
//
#include "core/Renderer.h"
#include "core/RenderContext.h"

RenderContext* RenderContext::_instance = nullptr;

void RenderContext::resetSurfaceSize(jobject javaRef, int width, int height) {
    JNIEnv *jniEnv;
    bool detached = jvm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6) == JNI_EDETACHED;
//    bool isAttached = false;
//    if (detached) {
//        isAttached = jvm->AttachCurrentThread(&jniEnv, nullptr);
//    }
    if (!detached) {
        jniEnv->CallVoidMethod(javaRef, renderClsInfo.resetSize, width, height);
    }
    // 非java线程才需要detach
//    if (isAttached) jvm->DetachCurrentThread();
}

