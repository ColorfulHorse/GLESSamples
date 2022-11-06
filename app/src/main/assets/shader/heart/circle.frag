#version 300 es
precision highp float;

out vec4 fragColor;
uniform vec2 layerSize;

// 画一个圆
void main() {
    // 坐标转换，转换为以短轴长度为基准，以中心点为原点的相对坐标
    // (xy - wh/2) / (短轴/2) =>（2xy - wh）/ 短轴
    vec2 uv = (2.0 * gl_FragCoord.xy - layerSize) / min(layerSize.y, layerSize.x);
    float r = length(uv);
    // 距离原点小于0.5的片段渲染为红色，smoothstep做边缘平滑
    // c = r < 0.5 => 1  r > 0.51 > 0
    float c = smoothstep(0.51, 0.5, r);
//    float c = (r > 0.4 && r < 0.5) ? 1.0 : 0.0;
    fragColor = vec4(.8,.2,0.3,0.0) * c;
}