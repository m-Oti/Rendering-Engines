#include "model.h"
#include <glm/gtc/matrix_transform.hpp>

namespace core {

    Model::Model(const core::Model& other) : modelMatrix(1), material(other.GetMaterial()) {
        //TODO the fun stuff
        this->meshes = other.meshes;
        this->modelMatrix = other.modelMatrix;
        printf("Copy ctor is being called");
    }

    void Model::render() {

        material.apply();
        for (int i = 0; i < meshes.size(); ++i) {
            meshes[i].render();
        }
    }

    Material Model::GetMaterial() const
    {
        return material;
    }

    void Model::SetMaterial(Material material)
    {
        this->material = material;
    }

    void Model::translate(glm::vec3 translation) {
        modelMatrix = glm::translate(modelMatrix, translation);
    }

    void Model::rotate(glm::vec3 axis, float radians) {
        modelMatrix = glm::rotate(modelMatrix, radians, axis);
    }

    void Model::scale(glm::vec3 scale) {
        modelMatrix = glm::scale(modelMatrix, scale);
    }

    glm::mat4 Model::getModelMatrix() const {
        return this->modelMatrix;
    }

    glm::vec3 Model::getGlobalScale() const
    {
        glm::mat4 m = getModelMatrix();

        return glm::vec3(
            glm::length(glm::vec3(m[0])),
            glm::length(glm::vec3(m[1])),
            glm::length(glm::vec3(m[2]))
        );
    }

    void Model::computeBBox() {
        for (const auto& mesh : meshes) {
            for (const auto& v : mesh.getVertices()) {  // was mesh.vertices
                bboxMin = glm::min(bboxMin, v.position);
                bboxMax = glm::max(bboxMax, v.position);
            }
        }
    }

    glm::mat4 Model::getBBoxModelMatrix() const {
        // The bbox verts are already in local space, so just apply the model matrix as-is
        // The box was built from the actual vertex extents, no extra scaling needed
        return modelMatrix;
    }

    void Model::renderBBox() {
        if (bboxVAO == 0) return; // computeBBox() was never called, skip
        glBindVertexArray(bboxVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}