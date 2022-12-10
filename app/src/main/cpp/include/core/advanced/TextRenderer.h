//
// Created by admin on 2022/10/12.
//

#ifndef GLSAMPLE_TEXTRENDERER_H
#define GLSAMPLE_TEXTRENDERER_H


#include "core/Renderer.h"
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint textureId;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    long advance;

    Character(GLuint textureId, const glm::ivec2 &size, const glm::ivec2 &bearing,
                         long advance) : textureId(textureId), size(size), bearing(bearing),
                                         advance(advance){}
};

class TextRenderer : public Renderer {
    virtual void onSurfaceCreated() override;

    virtual void onSurfaceDestroyed() override;

    virtual void onDraw() override;

private:
    const char* TAG = "TextRenderer";
    FT_Library ftLibrary;
    FT_Face ftFace;
    std::vector<Character> characters;

    void loadText(const std::u32string &text);

    void renderText(GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};


#endif //GLSAMPLE_TEXTRENDERER_H
