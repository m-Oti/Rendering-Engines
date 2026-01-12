#include "Scene.h"

Scene::Scene(std::string _name)
    : name(_name)
{}

void Scene::AddObj(core::Model* model)
{
    objs.push_back(model);
}


void Scene::renderScene(
    GLuint shader,
    const glm::mat4& view,
    const glm::mat4& projection,
    GLint mvpUniform,
    GLint modelUniform
)
{
    glUseProgram(shader);
    // TODO: What about all those other properties like tint? It should be possible that Game objects have different tint... Or a different texture...

    for (auto* model : objs)
    {
        glm::mat4 mvp = projection * view * model->getModelMatrix();
        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model->getModelMatrix()));
        model->render();
    }
}
