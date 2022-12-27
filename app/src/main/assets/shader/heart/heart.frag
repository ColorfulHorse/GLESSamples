#version 300 es
precision highp float;

out vec4 fragColor;
uniform vec2 layerSize;
uniform float delta;

const float PI = 3.141593;

void main() {
    // 坐标转换，转换为以短轴长度为基准，以中心点为原点的相对坐标
    // (xy - wh/2) / (短轴/2) =>（2xy - wh）/ 短轴
    vec2 uv = (2.0 * gl_FragCoord.xy - layerSize) / min(layerSize.y, layerSize.x);
    // 因为原本的uv坐标左上角为(0, 0)，所以这里的y轴是反的，我们矫正一下
    uv.y = - uv.y;
    // 背景色从中心到四周由浅到深
    vec3 bgColor = vec3(1.0, 0.8, 0.7 - 0.07 * -uv.y) * (1.0 - 0.25 * length(uv));

    // 观察一下这个动画，类似一个横向的挤压回弹效果
    // 那么我们需要在x方向上向内偏移，y方向上向外偏移
    // 另外需要这个偏移量遵循一个周期性的非线性变化
//    float ss = pow(delta, .2) * 0.5 + 0.5;
//    ss = 1.0 + ss * 0.5 * sin(delta * 6.2831 * 3.0 -uv.y * 0.5) * exp(-delta * 4.0);
//    uv *= (vec2(0.5, 1.5) + ss * vec2(0.5, -0.5));

    // heart beats
    float ss = (1.0 - 0.3/(delta + 0.3));
    uv *= 0.85 + ss * 0.15;

    uv.y += 0.25;
    // 片段坐标所在角的弧度，同一个方向上的片段atan值相同
    // atan2(x, y) [-PI,PI] => [-1, 1]
    float a = atan(uv.x, -uv.y) / PI;
    float h = abs(a);
    float r = length(uv);
    // 让形状看起来更接近爱心
    // 对h做一个非线性插值变换
    float d = (13.0 * h - 22.0 * h * h + 10.0 * h * h * h) / (6.0 - 5.0 * h);

    // 颜色插值，让爱心更有立体感
    float s = 0.75 + 0.75 * uv.x;
    s *= 1.0 - 0.4 * r;
    s = 0.3 + 0.7 * s;
    s *= 0.5 + 0.5 * pow(1.0 - clamp(r / d, 0.0, 1.0), 0.1);
    vec3 hcol = vec3(1.0, 0.4 * r, 0.3) * s;

    // d - r 的含义，已知相同方向的直线上d相同，那么r < d说明该片段在爱心内部，应该使用爱心的颜色
    // d - r在[-0.01, 0.01]区间内表示该片段在爱心边缘，做一个边缘平滑
    // smoothstep平滑过渡
    // t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // smoothstep(edge0, edge1, x) = (3.0 - 2.0 * t) * t * t;
//    vec3 col = r < d ? hcol : bgColor;
    // t[0,1]过渡，(1-t)*bgColor + t*hcol
    vec3 col = mix(bgColor, hcol, smoothstep(-0.01, 0.01, d - r));
    fragColor = vec4(col, 1.0);
}