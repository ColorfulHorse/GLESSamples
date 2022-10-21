//
// Created by admin on 2022/10/8.
//
#include <jni.h>
#include <android/asset_manager_jni.h>
#include "core/RenderContext.h"

template<class T>
int arrayLen(T &array) {
    return (sizeof(array) / sizeof(array[0]));
}

const char* cls_bridge = "com/greensun/glsample/RenderContext";

void set_assets_manager(JNIEnv *env, jclass cls, jobject assetManager) {
    auto manager = AAssetManager_fromJava(env, assetManager);
    RenderContext::instance()->assetManager = manager;
    env->GetJavaVM(&RenderContext::instance()->jvm);
}

void context_release(JNIEnv *env, jclass cls) {
    auto context = RenderContext::instance();
    delete context;
}


JNINativeMethod methods[] = {
    {"setAssetManager",  "(Landroid/content/res/AssetManager;)V", (void *) set_assets_manager},
    {"nativeRelease",  "()V", (void *) context_release},
//        {"setCallBack",  "(Lcom/lyj/learnffmpeg/PublishCallBack;)V", (void *) publisher_set_callback},
//        {"release",      "()V",                                      (void *) publisher_release},
//        {"startPublish", "(Ljava/lang/String;III)I",                 (void *) publisher_start_publish},
//        {"stopPublish",  "()I",                                      (void *) publisher_stop_publish},
//        {"publishData",  "([B)I",                                    (void *) publisher_publish_data}
};


int jniRegisterNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *methods,
                             int count) {
    int res = -1;
    jclass cls = env->FindClass(className);
    if (cls != nullptr) {
        int ret = env->RegisterNatives(cls, methods, count);
        if (ret > 0) {
            res = 0;
        }
    }
    env->DeleteLocalRef(cls);
    return res;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    jint result = -1;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
    jniRegisterNativeMethods(env, cls_bridge, methods, arrayLen(methods));
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved) {

}