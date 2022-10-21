//
// Created by admin on 2022/9/30.
//

#ifndef GLSAMPLE_RENDERER_H
#define GLSAMPLE_RENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>

#ifdef __cplusplus
}
#endif
#include <GLES3/gl32.h>
#include "Shader.h"

class Renderer {
protected:
    Shader shader;
    uint32_t VAO, VBO;
    int surfaceWidth, surfaceHeight;
public:
    jobject javaRef;
    virtual void onSurfaceCreated() = 0;
    virtual void onSurfaceChanged(int width, int height) {
        glViewport(0, 0, width, height);
        surfaceWidth = width;
        surfaceHeight = height;
    }
    virtual void onDraw() = 0;

    // 触摸点坐标，绝对坐标
    virtual void onTouch(float x, float y) {

    }
    virtual ~Renderer() {};
};


#endif //GLSAMPLE_RENDERER_H
