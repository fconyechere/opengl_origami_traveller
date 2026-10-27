# Deliverable 2

An OpenGL 3.3 starter project, organized into separate application, shader,
mesh, and model modules.

## Project layout

```text
Deliverable2/
├── assets/       Models, textures, and other runtime assets
├── shaders/      GLSL shader source files
├── src/
│   ├── main.cpp  Window setup, scene setup, and render loop
│   ├── Shader.h  Shader program interface
│   ├── Shader.cpp
│   ├── Mesh.h    GPU mesh interface
│   ├── Mesh.cpp
│   ├── Model.h   Collection of meshes
│   └── Model.cpp
├── assimp/       Existing local Assimp build output; not used yet
└── CMakeLists.txt
```

`main.cpp` creates a window and scene, then runs the render loop. `Shader` loads
and compiles the GLSL files; `Mesh` owns the vertex array and buffer; `Model`
groups meshes so the scene can draw them together. The current scene contains
one triangle. The `assets/` directory is ready for model and texture files, but
asset importing is not wired up yet.

The project builds with the GLFW and GLAD sources already included in
`Labs/lab4/external`.

## Build and run

From this directory:

```sh
cmake -S . -B build
cmake --build build
./build/deliverable2
```

Press **Escape** or close the window to exit.
