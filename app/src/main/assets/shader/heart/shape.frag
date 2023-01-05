#version 300 es
precision highp float;

out vec4 fragColor;
uniform vec2 layerSize;

// 推导 https://zhuanlan.zhihu.com/p/420700051

float sdfCircle(vec2 uv, vec2 center, float radius) {
    return distance(uv, center) - radius;
}

// 椭圆
float sdfEllipse(vec2 uv, vec2 center, float a, float b) {
    float a2 = a * a;
    float b2 = b * b;
    float dx = uv.x - center.x;
    float dy = uv.y - center.y;
    return (b2 * dx * dx + a2 * dy * dy - a2 * b2) / (a2 * b2);
}

// 圆环
float sdfRing(vec2 uv, vec2 center, float radius, float thickness) {
    // d = 0.6 r = 0.5 t = 0.2
    // -0.1
    return abs(distance(uv, center) - radius) - thickness;
}

// 矩形，这里width height为宽高的一半
float sdfBox(vec2 uv, vec2 center, float halfWidth, float halfHeight) {
    // uv = 0.8, 0.6 右上角  halfWidth = 0.8 halfHeight = 0.6  -0.4, -0.3
    // -0.3 + 0 = -0.3

    // uv = 0.3, 0.4 右上角  halfWidth = 0.8 halfHeight = 0.6  -0.5, -0.2
    // -0.2 + 0
    vec2 d = abs(uv - center) - vec2(halfWidth, halfHeight);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

// 圆角矩形
float sdfRoundBox(vec2 uv, vec2 center, float halfWidth, float halfHeight, float r) {
    // uv = 0.8, 0.6 右上角  halfWidth = 0.8 halfHeight = 0.6  0.1,0.1
    // 根号r - r     0 - r
    vec2 d = abs(uv - center) - vec2(halfWidth, halfHeight) + r;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

// 线段
float sdfLine(vec2 uv, vec2 a, vec2 b) {
    vec2 ap = uv - a;
    vec2 ab = b - a;
    float h = clamp(dot(ap, ab) / dot(ab, ab), 0.0, 1.0);
    return length(ap - h * ab);
}

vec4 render(float d, vec3 color, float strokeWidth) {
    float anti = fwidth(d) * 1.0;
    // d < 0 表明在图形内部, solid alpha = 1
    vec4 solid = vec4(color, 1.0 - smoothstep(-anti, anti, d));
    if (strokeWidth < 0.000001) {
        return solid;
    }

    // uvDistance = radius + d, strokeRadius = radius + strokeWidth => d - strokeWidth
    // d - strokeWidth < 0 表明在描边内部，stroke alpha = 1
    vec4 stroke = vec4(vec3(1.0, 1.0, 1.0), 1.0 - smoothstep(-anti, anti, d - strokeWidth));
    // 两次边缘平滑，填充-描边  描边-背景
    return vec4(mix(stroke.rgb, solid.rgb, solid.a), stroke.a);
}

// 画一个圆
void main() {
    // 坐标转换，转换为以短轴长度为基准，以中心点为原点的相对坐标
    // (xy - wh/2) / (短轴/2) =>（2xy - wh）/ 短轴
    vec2 uv = (2.0 * gl_FragCoord.xy - layerSize) / min(layerSize.y, layerSize.x);
    // 因为原本的uv坐标左上角为(0, 0)，所以这里的y轴是反的，我们矫正一下'
    //    float d = sdfEllipse(uv, vec2(0.0, 0.0), 0.8, 0.6);
    //    float d = sdfCircle(uv, vec2(0.0, 0.0), 0.5);
    //    float d = sdfRing(uv, vec2(0.0, 0.0), 0.45, 0.1);
    //    float d = sdfBox(uv, vec2(0.0, 0.0), 0.8, 0.5);
    //    float d = sdfRoundBox(uv, vec2(0.0, 0.0), 0.8, 0.5, 0.05);
    float d = sdfLine(uv, vec2(-0.4, 0.4), vec2(0.4, -0.4));
    vec4 color = render(d, vec3(0.91, 0.12, 0.39), fwidth(d) * 2.0);
//    fragColor = mix(vec4(0.1, 0.1, 0.1, 1.0), color, color.a);
    fragColor = vec4(vec3(uv.y), 1.0);
}