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
└── CMakeLists.txt
```

`main.cpp` creates a window and scene, then runs the render loop. `Shader` loads
and compiles the GLSL files; `Mesh` owns the vertex array and buffer; `Model`
groups imported meshes so the scene can draw them together. The current scene
loads `assets/table.obj` and `assets/notebook.obj` through Assimp. Their OBJ
material diffuse colors are passed to the fragment shader and combined with
the directional lighting.

The project builds with the GLFW, GLAD, and Assimp sources already included in
`Labs/lab4/external`. CMake builds Assimp under the selected build directory
and enables its OBJ and Collada importers. Assimp is linked to the application
and `Model` loads OBJ meshes and their normals and material colors. Run the
executable from the `Deliverable2` directory so the relative `assets/` and
`shaders/` paths resolve correctly.

## Build and run

From this directory:

```sh
cmake -S . -B build
cmake --build build
./build/deliverable2
```

Press **Escape** or close the window to exit.
