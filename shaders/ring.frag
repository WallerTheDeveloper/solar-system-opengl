#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D ringTexture;

void main() {
    vec4 texColor = texture(ringTexture, TexCoord);

    FragColor = texColor;

    // Discard fully transparent pixels to avoid z-fighting
    if (FragColor.a < 0.01) {
        discard;
    }
}