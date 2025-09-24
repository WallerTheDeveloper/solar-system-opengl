//#version 330 core
//out vec4 FragColor;
//
//in vec3 TexCoords;
//
//uniform samplerCube skybox;
//
//void main()
//{
//    // For testing without texture, just use a gradient based on position
//    vec3 color = normalize(TexCoords) * 0.5 + 0.5;
//    FragColor = vec4(color, 1.0);
//
//    // When you have textures, replace above with:
//    // FragColor = texture(skybox, TexCoords);
//}

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}