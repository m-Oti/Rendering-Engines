#version 400
in vec2 vuv;
uniform sampler2D textureUniform2;
out vec4 fragmentCol;
void main()
{
    if(framebuffer_is_active)
    {
        vec4 value = texture(textureUniform2, vuv);
        fragmentCol = texture(textureUniform2, vuv.st) + vec4(vuv, 0.0, 0.0); //vec4(0,0.1*sin(vuv.y*100),0,0); // 
    }else
    {
        fragmentCol = texture(textureUniform2, vuv);
        float average = 0.2126 * fragmentCol.r + 0.7152 * fragmentCol.g + 0.0722 * fragmentCol.b;
        fragmentCol = vec4(average, average, average, 1.0);
    }
}