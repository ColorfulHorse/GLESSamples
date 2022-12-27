#version 300 es
precision highp float;

out vec4 fragColor;
uniform vec2 layerSize;


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
    return abs(distance(uv, center) - radius) - thickness;
}

float sdfBox(vec2 uv, vec2 center, float width, float height) {
    vec2 d = abs(uv - center) - vec2(width, height);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

// 圆角矩形
float sdfRoundBox(vec2 uv, vec2 center, float width, float height, float r) {
    vec2 d = abs(uv - center) - vec2(width, height);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
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
    // 因为原本的uv坐标左上角为(0, 0)，所以这里的y轴是反的，我们矫正一下
    uv.y = -uv.y;
//    float d = sdfEllipse(uv, vec2(0.0, 0.0), 0.8, 0.6);
    //    float d = sdfCircle(uv, vec2(0.0, 0.0), 0.5);
    float d = sdfRing(uv, vec2(0.0, 0.0), 0.45, 0.1);
    vec4 ellipse = render(d, vec3(0.91, 0.12, 0.39), fwidth(d) * 2.0);
    fragColor = mix(vec4(0.1, 0.1, 0.1, 1.0), ellipse, ellipse.a);
}