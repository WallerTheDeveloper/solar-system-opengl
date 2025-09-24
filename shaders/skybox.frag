#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube skybox;

void main()
{
    // First, try a simple color test
    // Uncomment this line to see if geometry works:
    // FragColor = vec4(0.2, 0.5, 0.8, 1.0); // Light blue color

    // Try to sample the texture
    vec4 texColor = texture(skybox, TexCoords);

    // Debug: mix texture with a color so we can see something
    vec3 debugColor = normalize(TexCoords) * 0.5 + 0.5;

    // If texture sampling works, use it, otherwise use debug color
    if (length(texColor.rgb) > 0.01) {
        FragColor = texColor;
    } else {
        // Fallback to debug color if texture is black/empty
        FragColor = vec4(debugColor, 1.0);
    }

    // TEMPORARY: Force show debug colors to test geometry
    // Uncomment this line to force colorful gradient:
    FragColor = vec4(debugColor, 1.0);
}