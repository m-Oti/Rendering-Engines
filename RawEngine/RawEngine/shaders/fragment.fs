#version 400 core
uniform vec3 lightDirection; // world space
uniform mat4 mMatrixUniform;
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
    FragColor = dot(lightDirection, fNor) * vec4(1,1,1,0) + vec4(0.5,0,0.5,1);
   //FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}