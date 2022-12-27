//
// Created by admin on 2022/12/26.
//

#ifndef GLSAMPLE_GLYPHOUTLINE_H
#define GLSAMPLE_GLYPHOUTLINE_H

#include "../../glm/glm.hpp"
#include "../../ft2build.h"
#include FT_FREETYPE_H"../../freetype/freetype.h"
#include FT_OUTLINE_H"../../freetype/ftoutln.h"
#include FT_BBOX_H"../../freetype/ftbbox.h"
#include "../../../../../../../../../Library/Android/sdk/ndk/23.1.7779620/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/vector"

class GlyphOutline {
private:
    glm::vec2 current;
    // 0 moveTo 1 lineTo 2 conicTo 3 cubicTo
    std::vector<unsigned char> tags;
public:
    std::vector<glm::vec2> points;
    void decompose(FT_GlyphSlot glyph);
};


#endif //GLSAMPLE_GLYPHOUTLINE_H
