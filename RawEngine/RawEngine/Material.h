#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <glad/glad.h>
#include "core/texture.h"

class Material
{
    
public:
    std::shared_ptr<core::Texture> texture;
    Material(GLuint shader, glm::vec3 tint, float shininess);
    GLuint shaderProgram = 0;

    glm::vec3 tint = glm::vec3(1.0f);
    float shininess = 32.0f;

    std::shared_ptr<core::Texture> diffuseTexture;

    void apply() const;
    void SetTexture(std::shared_ptr<core::Texture> texure);
};
