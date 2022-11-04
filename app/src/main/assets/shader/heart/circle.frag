#version 300 es
precision highp float;

out vec4 fragColor;
uniform vec2 layerSize;

// 画一个圆
void main() {
    // 原点移到中心
    vec2 uv = (2.0 * gl_FragCoord.xy - layerSize) / min(layerSize.y, layerSize.x);
    // 半径
    float r = length(uv);
    // c = r < 0.5 => 1  r > 0.51 > 0
    // 半径小于0.5的片段渲染为红色，smoothstep做边缘平滑
    float c = smoothstep(0.51, 0.5, r);
//    float c = (r > 0.4 && r < 0.5) ? 1.0 : 0.0;
    fragColor = vec4(.8,.2,0.3,0.0) * c;
}