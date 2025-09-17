#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D skyboxTexture;

void main()
{
    vec4 texColor = texture(skyboxTexture, TexCoord);
    
    FragColor = texColor;
}