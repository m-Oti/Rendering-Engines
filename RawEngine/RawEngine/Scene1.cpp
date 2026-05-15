#include "Scene.h"

Scene::Scene(string _name) : name(_name) {}

void Scene::AddObj(core::Model* model) {
    objs.push_back(model);

    OcclusionQuery q;
    q.init();
    queries.push_back(q);
}

void Scene::collectQueryResults() {
    for (auto& q : queries)
        q.tryCollect();
}

void Scene::renderOcclusionPass(GLuint bboxShader,
    GLint bboxMvpUniform,
    const glm::mat4& view,
    const glm::mat4& projection)
{
    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDisable(GL_CULL_FACE);

    glUseProgram(bboxShader);

    for (int i = 0; i < objs.size(); i++) {
        glm::mat4 mvp = projection * view * objs[i]->getBBoxModelMatrix();
        glUniformMatrix4fv(bboxMvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

        queries[i].begin();
        objs[i]->renderBBox();
        queries[i].end();
    }

    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_CULL_FACE);
}

void Scene::renderScene(GLuint shader,
    const glm::mat4& view,
    const glm::mat4& projection,
    GLint mvpUniform,
    GLint modelUniform,
    const Frustum& frustum)
{
    glUseProgram(shader);
    for (int i = 0; i < objs.size(); i++) {
        // Build a bounding sphere from the model's bbox center and radius
        glm::vec3 center = (objs[i]->bboxMin + objs[i]->bboxMax) * 0.5f;
        float radius = glm::length(objs[i]->bboxMax - center);
        Sphere boundingSphere(center, radius);

        // Frustum cull first (cheap), then occlusion cull
        if (!boundingSphere.isOnFrustum(frustum, *objs[i])) continue;
        if (!queries[i].visibleLastFrame)                   continue;

        glm::mat4 mvp = projection * view * objs[i]->getModelMatrix();
        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(objs[i]->getModelMatrix()));
        objs[i]->render();
    }
}