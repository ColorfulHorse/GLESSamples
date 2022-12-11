#version 300 es

in vec2 textCoords;
out vec4 fragColor;
uniform vec3 textColor;
uniform sampler2D mTexture;

void main() {

    float red = texture(mTexture, textCoords).r;
    fragColor = vec4(textColor, 1.0) * vec4(1.0, 1.0, 1.0, red);
}