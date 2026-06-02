# OpenGL Rendering Engine — Graphics Programming Project

A real-time 3D/2D rendering engine built in **C++ and OpenGL 3.3**, developed on top of RawEngine V2.0 with self-written rendering, lighting, post-processing, and scene management code.

## Overview

The engine renders two independent scenes — one with 3D Assimp-loaded models and one mixing 2D/3D objects — switchable at runtime without restarting. The render loop is built around a custom framebuffer pipeline: each frame is rendered off-screen first, then passed through a post-processing shader before being drawn to screen. Two effects are implemented in GLSL: **Kernel Edge Detection** (convolution filter on the framebuffer texture) and **Colour Adjustment** (per-fragment brightness, contrast, and hue, tunable live via Dear ImGUI sliders).

Lighting uses the **ADS model** (Ambient, Diffuse, Specular) with attenuation, computed per-fragment in the shader with the light position, camera position, and material uniforms passed from the CPU. Models are loaded via Assimp, transformed with GLM matrices (Model → View → Projection), and textured through the shader program. Camera movement (WASD + Q/E + mouse look) is handled each frame before the view matrix is rebuilt. The engine is currently being extended with **frustum culling** — a `Frustum` struct is extracted from the camera each frame and passed to `Scene::renderScene()` to skip draw calls for objects outside the view volume, with occlusion query passes also in progress.

## Tech Stack

| | |
|---|---|
| **Language** | C++17 |
| **Graphics API** | OpenGL 3.3 Core Profile |
| **Shaders** | GLSL (vertex + fragment, per shader program) |
| **Libraries** | Assimp, GLM, GLFW3, GLAD, Dear ImGUI |
| **Build** | Visual Studio / CLion (cross-platform headers) |

## Key Systems

| File | Role |
|---|---|
| `RawEngine.cpp` | Main loop, shader program setup, framebuffer, scene switching |
| `Scene.cpp/.h` | Object list, occlusion query pass, `renderScene()` with frustum |
| `GameLogic.cpp/.h` | ADS lighting uniforms, material/texture binding |
| `Camera.cpp/.h` | Position, front vector, mouse rotation, frustum extraction |
| `FrustumCulling.cpp/.h` | Frustum struct, per-object AABB intersection test *(in progress)* |
| `shaders/framebuffer.fs` | Kernel Edge Detection post-processing |
| `shaders/framebuffer2.fs` | Colour Adjustment (brightness, contrast, hue) |

---

*C++ · OpenGL 3.3 · GLSL · Assimp · GLFW3 · Dear ImGUI*
