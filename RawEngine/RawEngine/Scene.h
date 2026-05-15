#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector> 
#include "GameObject.h"
#include "Camera.h"
#include <memory>
#include "core/model.h"
#include <string>
#include "FrustumCulling.h"
#include "OcclusionCulling.h"  // add this
using namespace std;

class Scene {
public:
    std::vector<core::Model*> objs;
    std::vector<OcclusionQuery> queries; // parallel array — same index as objs
    string name;

    Scene(string _name);

    void AddObj(core::Model* model);

    // Call at the start of each frame to collect last frame's results
    void collectQueryResults();

    // Draw bounding boxes with queries (color + depth writes off)
    void renderOcclusionPass(GLuint bboxShader,
        GLint bboxMvpUniform,
        const glm::mat4& view,
        const glm::mat4& projection);

    void renderScene(GLuint shader,
        const glm::mat4& view,
        const glm::mat4& projection,
        GLint mvpUniform,
        GLint modelUniform,
        const Frustum& frustum);
};