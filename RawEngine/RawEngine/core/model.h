#pragma once

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"


namespace core {
    class Model {
    private:
        // TODO: a Model has a reference to its Material HELLO :)
        // TODO: create Material class, which contains a reference to a shader ID, and uniform properties
        std::vector<core::Mesh> meshes;
        glm::mat4 modelMatrix;
    public:
        Model(std::vector<core::Mesh> meshes) : meshes(meshes), modelMatrix(1) {}
        Model(const core::Model& other); // copy constructor

        void render();

        void translate(glm::vec3 translation);
        void rotate(glm::vec3 axis, float radians);
        void scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
    };
}