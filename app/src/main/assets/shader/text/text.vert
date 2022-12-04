#version 300 es

layout(location = 0) in vec2 mPosition;
layout(location = 1) in vec2 mTextCoords;

out vec2 textCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(mPosition, 0.0, 1.0);
    textCoords = mTextCoords;
}