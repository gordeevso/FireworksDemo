#version 400 compatibility

out vec4 color;

uniform vec3 un_rectangle_color;

void main()
{
    color = vec4(un_rectangle_color, 1.0);
}