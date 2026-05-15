#pragma once

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"
#include "../Material.h"


namespace core {
    class Model {
    private:
        // TODO: a Model has a reference to its Material HELLO :)
        // TODO: create Material class, which contains a reference to a shader ID, and uniform properties
        std::vector<core::Mesh> meshes;
        glm::mat4 modelMatrix;
        
    public:
        Material material;

        Model(std::vector<core::Mesh> meshes) : meshes(meshes), modelMatrix(1), material(-1,glm::vec3(0.0),1) {}
        Model(const core::Model& other); // copy constructor

        // Add these member variables
        glm::vec3 bboxMin = glm::vec3(FLT_MAX);
        glm::vec3 bboxMax = glm::vec3(-FLT_MAX);
        GLuint bboxVAO = 0;
        GLuint bboxVBO = 0;
        GLuint bboxEBO = 0;

        // Add these methods
        void computeBBox();
        glm::mat4 getBBoxModelMatrix() const;
        void renderBBox();

        void render();
        Material GetMaterial() const;
        void SetMaterial(Material material);
        void translate(glm::vec3 translation);
        void rotate(glm::vec3 axis, float radians);
        void scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
        glm::vec3 getGlobalScale() const;
    };
}
