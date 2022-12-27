//
// Created by admin on 2022/12/26.
//

#ifndef GLSAMPLE_TEXTOUTLINERENDERER_H
#define GLSAMPLE_TEXTOUTLINERENDERER_H


#include "core/Renderer.h"
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "core/advanced/GlyphOutline.h"

class TextOutlineRenderer : public Renderer {
    virtual void onSurfaceCreated() override;

    virtual void onDraw() override;

    virtual ~TextOutlineRenderer();


private:
    const char* TAG = "TextOutlineRenderer";
    FT_Library ftLibrary;
    FT_Face ftFace;
    std::vector<glm::vec2> points;

    void loadText(const std::u32string &text);
};


#endif //GLSAMPLE_TEXTOUTLINERENDERER_H
