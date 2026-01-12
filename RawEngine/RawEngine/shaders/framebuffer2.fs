#version 400
in vec2 vuv;
uniform sampler2D textureUniform;
out vec4 frag_colour;
void main()
{
vec4 value = texture(textureUniform, vuv);
frag_colour = texture(textureUniform, vuv.st) + vec4(vuv, 0.0, 0.0);
}