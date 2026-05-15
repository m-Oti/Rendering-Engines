#pragma once
#include <glad/glad.h>
#include <vector>

struct OcclusionQuery {
    GLuint queryId = 0;
    bool   pending = false;   // query issued but not yet read
    bool   visibleLastFrame = true; // conservative: visible until proven otherwise

    void init() {
        glGenQueries(1, &queryId);
    }

    void destroy() {
        glDeleteQueries(1, &queryId);
    }

    // Call before drawing bounding box
    void begin() {
        glBeginQuery(GL_ANY_SAMPLES_PASSED, queryId);
        pending = true;
    }

    void end() {
        glEndQuery(GL_ANY_SAMPLES_PASSED);
    }

    // Call at the START of the next frame to collect results
    // Returns true if the result was available (non-blocking)
    bool tryCollect() {
        if (!pending) return true;

        GLint available = 0;
        glGetQueryObjectiv(queryId, GL_QUERY_RESULT_AVAILABLE, &available);
        if (!available) return false; // not ready yet, keep last result

        GLuint samplesPassed = 0;
        glGetQueryObjectuiv(queryId, GL_QUERY_RESULT, &samplesPassed);
        visibleLastFrame = (samplesPassed > 0);
        pending = false;
        return true;
    }
};