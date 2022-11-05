#include <string>
#include "core/basic/TriangleRenderer.h"
#include "core/advanced/RippleRenderer.h"
#include "core/advanced/HeartRenderer.h"
#include "core/render_type.h"

const char* renderClassName = "com/greensun/glsample/render/BaseRenderer";

/**
 * 从java对象存的地址获取native实例
 * @param env
 * @param obj
 * @return
 */
Renderer* getRenderer(JNIEnv *env, jobject obj) {
    jlong addr = env->GetLongField(obj, RenderContext::instance()->renderClsInfo.ptr);
    jint type = env->GetIntField(obj, RenderContext::instance()->renderClsInfo.type);
    Renderer* renderer = nullptr;
    switch (type) {
        case RENDER_TRIANGLE:
            renderer = reinterpret_cast<TriangleRenderer*>(addr);
            break;
        case RENDER_RIPPLE:
            renderer = reinterpret_cast<RippleRenderer*>(addr);
            break;
        case RENDER_HEART:
            renderer = reinterpret_cast<HeartRenderer*>(addr);
            break;
    }
    return renderer;
}

/**
 * 根据类型创建不同renderer
 * @param type
 * @return
 */
Renderer* createRenderer(jint type) {
    Renderer* renderer = nullptr;
    switch (type) {
        case RENDER_TRIANGLE:
            renderer = new TriangleRenderer();
            break;
        case RENDER_RIPPLE:
            renderer = new RippleRenderer();
            break;
        case RENDER_HEART:
            renderer = new HeartRenderer();
            break;
    }
    return renderer;
}

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>

JNIEXPORT void JNICALL
Java_com_greensun_glsample_render_BaseRenderer_00024Companion_nativeInit(JNIEnv *env, jobject thiz) {
    // 保存类信息方便之后反射
    jclass localClass = env->FindClass(renderClassName);
    jclass renderCls = reinterpret_cast<jclass>(env->NewGlobalRef(localClass));
    jfieldID renderPtr = env->GetFieldID(renderCls, "mNativePtr", "J");
    jfieldID renderType = env->GetFieldID(renderCls, "type", "I");
    jmethodID resetSize = env->GetMethodID(renderCls, "resetSurfaceSize", "(II)V");
    RenderContext::instance()->renderClsInfo.clz = renderCls;
    RenderContext::instance()->renderClsInfo.ptr = renderPtr;
    RenderContext::instance()->renderClsInfo.type = renderType;
    RenderContext::instance()->renderClsInfo.resetSize = resetSize;
}

JNIEXPORT jlong JNICALL
Java_com_greensun_glsample_render_BaseRenderer_nativeCreate(JNIEnv *env, jobject obj, jint type) {
    auto renderer = createRenderer(type);
    renderer->javaRef = env->NewGlobalRef(obj);
    return reinterpret_cast<jlong>(renderer);
}

JNIEXPORT void JNICALL
Java_com_greensun_glsample_render_BaseRenderer_nativeDestroy(JNIEnv *env, jobject obj) {
    auto renderer = getRenderer(env, obj);
    if (renderer) {
        env->DeleteGlobalRef(renderer->javaRef);
        delete renderer;
    }
}

JNIEXPORT void JNICALL
Java_com_greensun_glsample_render_BaseRenderer_nativeOnSurfaceCreated(JNIEnv *env, jobject obj) {
    getRenderer(env, obj)->onSurfaceCreated();
}

JNIEXPORT void JNICALL
Java_com_greensun_glsample_render_BaseRenderer_nativeOnSurfaceChanged(JNIEnv *env, jobject obj, jint width, jint height) {
    getRenderer(env, obj)->onSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_greensun_glsample_render_BaseRenderer_nativeOnDrawFrame(JNIEnv *env, jobject obj) {
    getRenderer(env, obj)->onDraw();
}

JNIEXPORT void JNICALL
Java_com_greensun_glsample_render_BaseRenderer_nativeTouch(JNIEnv *env, jobject obj, jfloat x,
                                                           jfloat y) {
    getRenderer(env, obj)->onTouch(x, y);
}

#ifdef __cplusplus
}
#endif
