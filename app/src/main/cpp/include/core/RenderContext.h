//
// Created by admin on 2022/10/8.
//

#ifndef GLSAMPLE_RENDERCONTEXT_H
#define GLSAMPLE_RENDERCONTEXT_H

#include <android/asset_manager.h>
#include <jni.h>

class RenderContext {
private:
    static RenderContext *_instance;

    RenderContext() {};

    RenderContext(const RenderContext &) {};

    RenderContext &operator=(const RenderContext &) {
        return *this;
    };

public:
    JavaVM *jvm = nullptr;

    static RenderContext *instance() {
        if (_instance == nullptr) {
            _instance = new RenderContext();
        }
        return _instance;
    }

    AAssetManager *assetManager = nullptr;

    void resetSurfaceSize(jobject javaRef, int width, int height);

    struct ClsInfo {
        jclass clz;
        jfieldID ptr;
        jfieldID type;
        jmethodID resetSize;
    } renderClsInfo;


    ~RenderContext() {
        assetManager = nullptr;
        jvm = nullptr;
    }
};


#endif //GLSAMPLE_RENDERCONTEXT_H
