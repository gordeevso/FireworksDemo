#version 400 compatibility
out vec4 Color;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    Color = texture(screenTexture, TexCoords);
}