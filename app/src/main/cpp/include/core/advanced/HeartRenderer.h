//
// Created by admin on 2022/10/12.
//

#ifndef GLSAMPLE_HEARTRENDERER_H
#define GLSAMPLE_HEARTRENDERER_H


#include "core/Renderer.h"

class HeartRenderer : public Renderer {
    virtual void onSurfaceCreated() override;

    virtual void onSurfaceDestroyed() override;

    virtual void onDraw() override;

private:
};


#endif //GLSAMPLE_HEARTRENDERER_H
