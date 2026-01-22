#version 400 core
uniform vec3 lightDirection; // world space
// TODO: add ambient light uniform

uniform vec3 tint; // part of material
uniform vec4 objectColor;  // part of material

uniform vec4 lightColor;
uniform float ambientIntensity;
uniform vec4 ambientLightColor;
uniform vec3 lightPos;

uniform vec3 camPos;
uniform vec4 specularColor;
uniform float shininess; // part of material
uniform sampler2D uvGridText; // part of material
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;
in mat3 TBN;

void main()
{
//FragColor = vec4(uv,0,1);
//return;
// debug:
    //FragColor = vec4(lightDirection,1);
    // TODO: light calculations with dot product
    
    // max(diffuseTerm, 0) * objectColour * lightColour 
    vec4 textColor = texture (uvGridText, uv);

    vec4 diffuseLight = max(dot(lightDirection, fNor),0) * lightColor; //vec4(1,1,1,1);
    //diffuseLight.a = 1.0;

    vec4 ambient = ambientIntensity * ambientLightColor;
    vec3 l = normalize(lightPos - fPos);
    vec3 reflectedLight = reflect(l, fNor);
    vec3 distance = normalize(fPos - camPos);

    float s = max(dot(reflectedLight, distance), 0);
    vec4 specular = pow(s, shininess) * specularColor * vec4 (0.1f, 0.1f, 1, 1);
    //specular.a = 1.0;
    //vec3 constDist = (0.234 + 0.153 * distance + 0.747 * distance * distance);
    //float attenuation = diffuseLight/(0.234 + 0.153 * distance + 0.747 * distance * distance);

    FragColor = (diffuseLight + ambient + specular) + vec4(textColor.rgb + tint, 1.0);
    FragColor.a=1;
    //FragColor = specular;
    //FragColor = vec4(pow(s, shininess), pow(s, shininess), pow(s, shininess), 1);
    //FragColor = vec4(reflectedLight, 1.0);
   //FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);

   //vec4 result = ambientTerm + (diffuseTerm + specularTerm) * attenuation;
   //result.a = 1.0;
   //FragColour = result;
}