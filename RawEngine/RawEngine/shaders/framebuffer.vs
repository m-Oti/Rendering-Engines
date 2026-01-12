#version 400
layout (location = 0) in vec3 vertex;
layout (location = 2) in vec2 uv;
out vec2 vuv;
void main( )
{
vuv = uv;
gl_Position = vec4(vertex, 1.0);
};