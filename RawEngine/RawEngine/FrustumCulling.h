#pragma once

#include <glm/glm.hpp>
#include "Camera.h"
#include "core/model.h"

// -------------------- Plane --------------------
struct Plane
{
    glm::vec3 normal;
    glm::vec3 point;

    Plane() = default;

    Plane(const glm::vec3& p, const glm::vec3& n)
        : point(p), normal(glm::normalize(n)) {}

    float getSignedDistanceToPlane(const glm::vec3& p) const
    {
        return glm::dot(normal, p - point);
    }
};

// -------------------- Frustum --------------------
struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
};

// -------------------- Sphere --------------------
struct Sphere
{
    glm::vec3 center{ 0.0f };
    float radius{ 1.0f };

    Sphere() = default;

    Sphere(const glm::vec3& c, float r)
        : center(c), radius(r) {}

    bool isOnOrForwardPlane(const Plane& plane) const
    {
        return plane.getSignedDistanceToPlane(center) > -radius;
    }

    bool isOnFrustum(const Frustum& frustum, const core::Model& model) const
    {
        glm::vec3 scale = model.getGlobalScale();

        glm::vec3 globalCenter =
            glm::vec3(model.getModelMatrix() * glm::vec4(center, 1.0f));

        float maxScale = std::max({ scale.x, scale.y, scale.z });

        Sphere globalSphere(globalCenter, radius * maxScale * 0.5f);

        return (
            globalSphere.isOnOrForwardPlane(frustum.leftFace) &&
            globalSphere.isOnOrForwardPlane(frustum.rightFace) &&
            globalSphere.isOnOrForwardPlane(frustum.topFace) &&
            globalSphere.isOnOrForwardPlane(frustum.bottomFace) &&
            globalSphere.isOnOrForwardPlane(frustum.nearFace) &&
            globalSphere.isOnOrForwardPlane(frustum.farFace)
            );
    }
};

// -------------------- Frustum Creation --------------------
inline Frustum createFrustumFromCamera(
    const Camera& cam,
    float aspect,
    float fovY,
    float zNear,
    float zFar)
{
    Frustum frustum;

    const float halfVSide = zFar * tanf(fovY * 0.5f);
    const float halfHSide = halfVSide * aspect;

    const glm::vec3 front = glm::normalize(cam.cameraFront);
    const glm::vec3 right = glm::normalize(cam.cameraRight);
    const glm::vec3 up = glm::normalize(cam.cameraUp);

    const glm::vec3 pos = cam.cameraPos;
    const glm::vec3 frontFar = front * zFar;

    frustum.nearFace = Plane(pos + front * zNear, front);
    frustum.farFace = Plane(pos + frontFar, -front);

    frustum.rightFace = Plane(pos,
        glm::cross(frontFar - right * halfHSide, up));

    frustum.leftFace = Plane(pos,
        glm::cross(up, frontFar + right * halfHSide));

    frustum.topFace = Plane(pos,
        glm::cross(right, frontFar - up * halfVSide));

    frustum.bottomFace = Plane(pos,
        glm::cross(frontFar + up * halfVSide, right));

    return frustum;
}