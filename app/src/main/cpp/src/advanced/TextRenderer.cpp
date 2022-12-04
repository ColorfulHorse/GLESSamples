//
// Created by admin on 2022/10/12.
//

#include <glm/glm.hpp>
#include <chrono>
#include "core/advanced/TextRenderer.h"
#include "GLUtils.h"


void TextRenderer::onSurfaceCreated() {
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
    loadText(U"这个是文字");
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    shader = Shader("shader/text/text.vert", "shader/text/text.frag");


    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    shader.use();
    shader.setInt("textTexture", 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::loadText(const std::u32string &text) {
    for (auto iter = text.cbegin(); iter != text.cend() ; ++iter) {
        //    FT_Set_Char_Size(
//            ftFace,    /* handle to face object         */
//            0,       /* char_width in 1/64 of points  */
//            16*64,   /* char_height in 1/64 of points */
//            300,     /* horizontal device resolution  */
//            300 );   /* vertical device resolution    */
        FT_Set_Pixel_Sizes(ftFace, 48, 0);

//    FT_UInt glyphIndex = FT_Get_Char_Index(ftFace, 0x3002);
//    FT_Load_Glyph(ftFace, glyphIndex, FT_LOAD_DEFAULT);
        FT_Error ret = FT_Load_Char(ftFace, *iter, FT_LOAD_DEFAULT);
        if (ret != FT_Err_Ok) {
            LOGE(TAG, "load char error");
            return;
        }
        // 默认渲染为8bit 256灰度bitmap
        FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        // 根据图片创建纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     ftFace->glyph->bitmap.buffer);
        // 创建多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        characters.emplace_back(
                textureId,
                glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
                glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
                ftFace->glyph->advance.x
                );
    }
}

void TextRenderer::onDraw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderText(25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
}

void TextRenderer::renderText(GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    shader.use();
    shader.setVec3("textColor", color);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(surfaceWidth), 0.0f, static_cast<float>(surfaceHeight));
    shader.setMat4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    for (auto iter = characters.cbegin(); iter != characters.cend(); ++iter) {
        Character c = *iter;
        // 字体框左下角
        GLfloat left = x + c.bearing.x * scale;
        GLfloat bottom = y - (c.size.y - c.bearing.y) * scale;
        GLfloat right = c.size.x * scale + left;
        GLfloat top = c.size.y * scale + bottom;
        float vertices[] = {
                left, bottom, 0.0f, 0.0f,
                right, top, 1.0f, 1.0f,
                left, top, 0.0f, 1.0f,
                left, bottom, 0.0f, 0.0f,
                right, bottom, 1.0f, 0.0f,
                right, top, 1.0f, 1.0f,
        };

//        GLfloat xpos = x + c.bearing.x * scale;
//        GLfloat ypos = y - (c.size.y - c.bearing.y) * scale;
//        GLfloat w = c.size.x * scale;
//        GLfloat h = c.size.y * scale;
//        // Update VBO for each character
//        GLfloat vertices[] = {
//                 xpos,     ypos + h,   0.0, 0.0 ,
//                 xpos,     ypos,       0.0, 1.0 ,
//                 xpos + w, ypos,       1.0, 1.0 ,
//
//                 xpos,     ypos + h,   0.0, 0.0 ,
//                 xpos + w, ypos,       1.0, 1.0 ,
//                 xpos + w, ypos + h,   1.0, 0.0
//        };

        glBindTexture(GL_TEXTURE_2D, c.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // advance的值是1/64像素
        x += (c.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextRenderer::~TextRenderer() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(0, &VAO);
    shader.release();
}

