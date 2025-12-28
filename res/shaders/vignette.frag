#version 100

precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 resolution;

void main() {
    vec4 color = texture2D(texture0, fragTexCoord);

    vec2 uv = fragTexCoord * 2.0 - 1.0;
    float dist = length(uv);
    float vignette = smoothstep(1.8, 0.8, dist);
    vignette = mix(0.2, 1.0, vignette);

    vec3 vignetted = mix(vec3(0.0), color.rgb, vignette);
    gl_FragColor = vec4(vignetted, color.a) * colDiffuse * fragColor;
}
