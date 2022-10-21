#version 300 es
precision highp float;

uniform sampler2D mTexture;
uniform vec2 touchPos;
uniform float radius;
uniform float boundary;
uniform float layer_width;
uniform float layer_height;

in vec2 texCoords;

out vec4 fragColor;


void main() {
    float ratio = layer_height / layer_width;
    vec2 textureCoords = texCoords * vec2(1.0, ratio);
    vec2 touchXY = touchPos * vec2(1.0, ratio);
    float dis = distance(touchXY, textureCoords);
    if (radius - boundary > 0.0 &&
    dis > radius - boundary &&
    dis < radius + boundary) {
        float delta = dis - radius;
        // 采样偏移，内圈<0 外圈>0
        float offset = 20.0 * delta * (delta - boundary) * (delta + boundary);
        fragColor = texture(mTexture, textureCoords);
        // 波纹方向
        vec2 direct = normalize(textureCoords - touchXY);
        textureCoords = textureCoords + (direct * offset);
    }
    textureCoords = textureCoords / vec2(1.0, ratio);
    fragColor = texture(mTexture, textureCoords);
}