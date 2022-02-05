#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec2 aTextureCoord;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    TexCoords = aTextureCoord;
}




#shader fragment
#version 330 core

uniform sampler2D Scene;


in vec2 TexCoords;
out vec4 FragColor;





float sobel()
{
    mat3 sx = mat3(
        1.0, 2.0, 1.0,
        0.0, 0.0, 0.0,
        -1.0, -2.0, -1.0
    );
    mat3 sy = mat3(
        1.0, 0.0, -1.0,
        2.0, 0.0, -2.0,
        1.0, 0.0, -1.0
    );



    mat3 I;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vec3 sample = texelFetch(Scene, ivec2(gl_FragCoord) + ivec2(i - 1, j - 1), 0).rgb;
            I[i][j] = length(sample);
        }
    }

    float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]);
    float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

    return sqrt(pow(gx, 2.0) + pow(gy, 2.0));
}



// Credit for these two functions:
// http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = c.g < c.b ? vec4(c.bg, K.wz) : vec4(c.gb, K.xy);
    vec4 q = c.r < p.x ? vec4(p.xyw, c.r) : vec4(c.r, p.yzx);

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0, 1), c.y);
}





void main()
{
    vec3 hsvTex = rgb2hsv(texture(Scene, TexCoords).rgb);
    hsvTex.y = 1.0;		// Modify saturation.
    hsvTex.z = 1.0;		// Modify lightness/value.
    vec3 col = hsv2rgb(hsvTex);

    // both edge detection and NPR
    vec3 hdrColor = col * sobel();

    FragColor = vec4(hdrColor, 1.0);
}