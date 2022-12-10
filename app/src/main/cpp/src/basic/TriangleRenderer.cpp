//
// Created by admin on 2022/9/30.
//

#include "core/basic/TriangleRenderer.h"

void TriangleRenderer::onSurfaceCreated() {
    float vertices[] = {
        0.5f, -0.5f,
        -0.5f, -0.5f,
        0.0f, 0.5f
    };
    shader = Shader("shader/basic/triangle.vert", "shader/basic/triangle.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    shader.use();
}

void TriangleRenderer::onDraw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

TriangleRenderer::~TriangleRenderer() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

