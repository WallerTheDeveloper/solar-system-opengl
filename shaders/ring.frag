#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D ringTexture;

void main() {
    vec4 texColor = texture(ringTexture, TexCoord);

    float dist = distance(TexCoord, vec2(0.5, 0.5));
    float alpha = texColor.a * (1.0 - smoothstep(0.3, 0.5, dist));

    FragColor = vec4(texColor.rgb, alpha);
}