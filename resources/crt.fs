#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution;   // screen resolution in pixels (or output size)
uniform float time;

// Tune these
uniform float curvature;   // 0.0 .. 0.25  (try 0.10)
uniform float scanline;    // 0.0 .. 1.0   (try 0.35)
uniform float vignette;    // 0.0 .. 1.0   (try 0.35)
uniform float aberration;  // 0.0 .. 2.0   (try 0.90)
uniform float grille;      // 0.0 .. 1.0   (try 0.30)

vec2 curveUV(vec2 uv, float amount)
{
    // map [0,1] -> [-1,1]
    vec2 p = uv * 2.0 - 1.0;

    // simple barrel distortion
    float r2 = dot(p, p);
    p *= 1.0 + amount * r2;

    // back to [0,1]
    return (p * 0.5 + 0.5);
}

void main()
{
    vec2 uv = fragTexCoord;

    // Curvature
    uv = curveUV(uv, curvature);

    // Outside curved area -> black
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
    {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // Chromatic aberration (offset in x)
    float off = aberration / resolution.x; // scale with width
    vec3 col;
    col.r = texture(texture0, uv + vec2( off, 0.0)).r;
    col.g = texture(texture0, uv).g;
    col.b = texture(texture0, uv + vec2(-off, 0.0)).b;

    // Scanlines (horizontal)
    float y = uv.y * resolution.y;
    float scan = 0.5 + 0.5 * sin(y * 3.14159);
    col *= mix(1.0, scan, scanline);

    // Shadow mask / grille (vertical)
    float x = uv.x * resolution.x;
    float mask = 0.66 + 0.34 * sin(x * 2.0 * 3.14159);
    col *= mix(1.0, mask, grille);

    // Vignette
    vec2 d = uv - 0.5;
    float vig = 1.0 - vignette * smoothstep(0.2, 0.85, dot(d, d) * 1.6);
    col *= vig;

    // Slight bloom-ish lift (optional mild)
    col = pow(col, vec3(0.95));

    finalColor = vec4(col, 1.0);
}
