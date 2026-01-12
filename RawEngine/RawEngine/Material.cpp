#include "Material.h"
#include "core/texture.h"

Material::Material(GLuint shader, glm::vec3 tint, float shininess)
{
    this->shaderProgram = shader;
    this->tint = tint;
    this->shininess = shininess;
}

void Material::apply() const
{
    glUseProgram(shaderProgram);

    GLint tintLoc = glGetUniformLocation(shaderProgram, "tint");
    if (tintLoc != -1)
        glUniform3fv(tintLoc, 1, &tint[0]);

    GLint shininessLoc = glGetUniformLocation(shaderProgram, "shininess");
    if (shininessLoc != -1)
        glUniform1f(shininessLoc, shininess);

    if (diffuseTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture->getId());

        GLint texLoc = glGetUniformLocation(shaderProgram, "uvGridText");
        if (texLoc != -1)
            glUniform1i(texLoc, 0);
    }
}

void Material::SetTexture(std::shared_ptr<core::Texture> texture)
{
    this->texture = texture;
}
