//
// Created by admin on 2022/10/12.
//

#include <glm/glm.hpp>
#include "core/advanced/RippleRenderer.h"
#include "GLUtils.h"


void RippleRenderer::onSurfaceCreated() {
    stbi_set_flip_vertically_on_load(true);
    int width, height;
    texture = GLUtils::loadAssetsTexture("texture/sight.jpeg", &width, &height, GL_CLAMP_TO_EDGE);
    RenderContext::instance()->resetSurfaceSize(javaRef, width, height);
    shader = Shader("shader/ripple/ripple.vert", "shader/ripple/ripple.frag");

    float vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,

            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 1.0f
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    shader.use();
    shader.setInt("mTexture", 0);
    shader.setFloat("boundary", boundary);
}

void RippleRenderer::onDraw() {
    float radius = 2.0f - 2.0f / (0.01f * frameIndex + 1);
    shader.setFloat("radius", radius);
    shader.setFloat("layer_width", surfaceWidth);
    shader.setFloat("layer_height", surfaceHeight);
    shader.setVec2("touchPos", glm::vec2(touchX, touchY));
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    if (frameIndex == -1) return;
    frameIndex++;
}

void RippleRenderer::onTouch(float x, float y) {
    frameIndex = 0;
    touchX = x / surfaceWidth;
    // 坐标系转换
    touchY = (surfaceHeight - y) / surfaceHeight;
}

void RippleRenderer::onSurfaceDestroyed() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(0, &VAO);
    glDeleteTextures(1, &texture);
    shader.release();
}

