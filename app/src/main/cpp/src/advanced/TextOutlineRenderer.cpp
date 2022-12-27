//
// Created by admin on 2022/12/26.
//

#include "core/advanced/TextOutlineRenderer.h"

void TextOutlineRenderer::onSurfaceCreated() {
    FT_Error ret = FT_Init_FreeType(&ftLibrary);
    if (ret != FT_Err_Ok) {
        LOGE(TAG, "init ft lib error");
        return;
    }
    long len;
    auto bytes = AssetUtil::loadAsset("fonts/fzkt.ttf", &len);
    ret = FT_New_Memory_Face(ftLibrary, bytes, len, 0, &ftFace);
    if (ret != FT_Err_Ok) {
        LOGE(TAG, "init ft face error");
        return;
    }

    // 内存对齐设为1
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    loadText(U"这个是文字wqeq123");

    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenBuffers(1, &VBO);
}

void TextOutlineRenderer::onDraw() {

}

TextOutlineRenderer::~TextOutlineRenderer() {

}

void TextOutlineRenderer::loadText(const std::u32string &text) {
    std::vector<glm::vec2> vec;
    points.swap(vec);
    FT_Set_Pixel_Sizes(ftFace, 50, 50);

    for (auto iter = text.cbegin(); iter != text.cend(); ++iter) {
        FT_Error ret = FT_Load_Char(ftFace, *iter, FT_LOAD_DEFAULT);
        if (ret != FT_Err_Ok) {
            LOGE(TAG, "load char error");
            return;
        }
        GlyphOutline outline;
        outline.decompose(ftFace->glyph);
        points.insert(points.end(), outline.points.begin(),  outline.points.end());
    }
}
