#version 300 es
layout(location = 0) in vec2 mPosition;
layout(location = 1) in vec2 mTexCoords;

out vec2 texCoords;


void main() {
    texCoords = mTexCoords;
    gl_Position = vec4(mPosition, 0.0, 1.0);
}