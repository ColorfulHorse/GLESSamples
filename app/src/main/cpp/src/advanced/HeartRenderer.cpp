//
// Created by admin on 2022/10/12.
//

#include <glm/glm.hpp>
#include <chrono>
#include "core/advanced/HeartRenderer.h"
#include "GLUtils.h"


void HeartRenderer::onSurfaceCreated() {
    shader = Shader("shader/heart/heart.vert", "shader/heart/heart.frag");

    float vertices[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f, 1.0f,

            1.0f, -1.0f,
            1.0f, 1.0f,
            -1.0f, 1.0f,
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
    shader.use();
}

void HeartRenderer::onDraw() {
    using namespace std::chrono;
    auto t = system_clock::now();
    auto tt = duration_cast<duration<long long, std::ratio<1, 1000>>>(t.time_since_epoch()).count();
    auto delta = static_cast<float>(fmod(tt, 2000) / 2000);
//    LOGE("HeartRenderer", "time: %lld", tt);
//    LOGE("HeartRenderer", "delta: %f", delta);
    shader.setVec2("layerSize", glm::vec2(surfaceWidth, surfaceHeight));
    shader.setFloat("delta", delta);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void HeartRenderer::onSurfaceDestroyed() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(0, &VAO);
    glDeleteTextures(1, &texture);
    shader.release();
}

