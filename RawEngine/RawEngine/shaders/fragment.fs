#version 400 core
uniform vec3 lightDirection;

uniform vec3 tint;
uniform vec4 objectColor;

uniform vec4 lightColor;
uniform float ambientIntensity;
uniform vec4 ambientLightColor;
uniform vec3 lightPos;

uniform vec3 camPos;
uniform vec4 specularColor;
uniform float shininess;
uniform sampler2D uvGridText;
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

void main()
{
    vec3 norm = normalize(fNor);
    vec3 lightDir = normalize(lightPos - fPos);
    vec3 viewDir = normalize(camPos - fPos);

    //Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    //Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = spec * specularColor;

    //Attenuation
    float distance = length(lightPos - fPos);
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    // Apply attenuation
    diffuse *= attenuation;
    specular *= attenuation;

    //Ambient (not attenuated)
    vec4 ambient = ambientIntensity * ambientLightColor;

    //Final color
    vec4 textColor = texture(uvGridText, uv);
    vec4 result = ambient + diffuse + specular;
    result.rgb *= (textColor.rgb + tint);

    FragColor = vec4(result.rgb, 1.0);
}