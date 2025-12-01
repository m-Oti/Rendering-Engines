#version 400 core
uniform vec3 lightDirection; // world space
// TODO: add ambient light uniform
uniform vec3 tint;
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

void main()
{
// debug:
    //FragColor = vec4(lightDirection,1);
    // TODO: light calculations with dot product
    vec4 colour = dot(lightDirection, fNor) * vec4(1,1,1,1);
    colour.a = 1.0;
    FragColor = colour;
   //FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}