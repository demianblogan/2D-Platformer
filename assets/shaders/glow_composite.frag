// Additive bloom composite that keeps the aura's hue. The halo texture holds
// premultiplied color (rgb = color * spill, a = spill). Boosting the spill
// coverage — capped at 1 — instead of stacking additive passes keeps the
// strong inner aura at the chosen color; stacked adds would clamp every
// channel and wash it out to white.

uniform sampler2D texture;
uniform float boost;     // spill coverage multiplier
uniform float intensity; // final brightness (drives the pulse)

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    if (pixel.a < 0.003)
    {
        gl_FragColor = vec4(0.0);
        return;
    }

    vec3 color = pixel.rgb / pixel.a; // un-premultiply back to the pure color
    float spill = min(pixel.a * boost, 1.0);

    gl_FragColor = vec4(color * spill * intensity, 1.0);
}
