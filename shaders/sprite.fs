#version 400 compatibility

// fragment shader for sprite rendering
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
}