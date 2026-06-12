// Draws the texture as a flat silhouette of a single color, keeping only its
// alpha. Used to feed the bloom layer when the aura color should not depend
// on the sprite's own pixels (e.g. a pink halo around a green melon).

uniform sampler2D texture;
uniform vec3 glowColor;

void main()
{
    float alpha = texture2D(texture, gl_TexCoord[0].xy).a * gl_Color.a;
    gl_FragColor = vec4(glowColor, alpha);
}
