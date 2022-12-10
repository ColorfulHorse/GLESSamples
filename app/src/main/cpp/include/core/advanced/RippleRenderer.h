//
// Created by admin on 2022/10/12.
//

#ifndef GLSAMPLE_RIPPLERENDERER_H
#define GLSAMPLE_RIPPLERENDERER_H


#include "core/Renderer.h"

class RippleRenderer : public Renderer {
    virtual void onSurfaceCreated() override;

    virtual void onSurfaceDestroyed() override;

    virtual void onDraw() override;

    virtual void onTouch(float x, float y) override;

private:
    float boundary = 0.1f;
    // 触摸点相对坐标
    float touchX = 0.5f;
    float touchY = 0.5f;
    // 触摸后的帧数
    int frameIndex = -1;

    uint32_t texture = 0;
};


#endif //GLSAMPLE_RIPPLERENDERER_H
