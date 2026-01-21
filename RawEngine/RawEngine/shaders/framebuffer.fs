#version 400
in vec2 vuv;
uniform sampler2D textureUniform;
uniform bool framebuffer_is_active;
out vec4 frag_colour;

void main() 
{
    float offset = 0.001;
    vec2 offsets[9] = vec2[ ](
    vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
    vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2( offset, 0.0f),
    vec2(-offset, -offset), vec2(0.0f, -offset), vec2( offset, -offset)
);
float kernel[9] = float[ ](
        1.0, 1.0, 1.0,
        1.0, -8.0, 1.0,
        1.0, 1.0, 1.0
);
vec4 sampleTex[9];
vec4 col = vec4(0.0);
for(int i = 0; i < 9; i++)
{
col += texture(textureUniform, vuv.st + offsets[i]) * kernel[i];
}
col.a = 1.0;
frag_colour = col;
}


//in vec2 vuv;
//uniform sampler2D textureUniform;
//uniform bool framebuffer_is_active;
//out vec4 frag_colour;
//void main()
//{
//    if(framebuffer_is_active)
//    {
//        vec4 value = texture(textureUniform, vuv);
//        frag_colour = texture(textureUniform, vuv.st) + vec4(vuv, 0.0, 0.0); //vec4(0,0.1*sin(vuv.y*100),0,0); // 
//    }else 
//    {
//        frag_colour = texture(textureUniform, vuv);
//        float average = 0.2126 * frag_colour.r + 0.7152 * frag_colour.g + 0.0722 * frag_colour.b;
//        frag_colour = vec4(average, average, average, 1.0);
//    }
//}