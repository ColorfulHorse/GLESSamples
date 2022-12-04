#version 300 es

in vec2 textCoords;
out vec4 fragColor;
uniform vec3 textColor;
uniform sampler2D textTexture;

void main() {
//    fragColor = vec4(textColor, 1.0);
    fragColor = vec4(vec3(texture(textTexture, textCoords).r), 1.0);
//    fragColor = vec4(textColor, 1.0) * vec4(1.0, 1.0, 1.0, texture(textTexture, textCoords).r);
}