//
// Created by admin on 2022/9/30.
//

#ifndef GLSAMPLE_TRIANGLERENDERER_H
#define GLSAMPLE_TRIANGLERENDERER_H

#include "core/Renderer.h"

class TriangleRenderer : public Renderer {
    virtual void onSurfaceCreated() override;
    virtual void onDraw() override;
    virtual ~TriangleRenderer();
};


#endif //GLSAMPLE_TRIANGLERENDERER_H
