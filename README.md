# Procedural Terrain Generator (OpenGL)

A small OpenGL project that generates terrain in real-time using noise.  
Includes multiple rendering modes, dynamic lighting, and an ImGui UI for tweaking parameters.

![Terrain Overview](media/Screenshot%20from%202025-08-19%2018-02-01.png)

---

## Features
- Procedural terrain with FastNoiseLite
- Wireframe / solid / point cloud modes
- First-person camera (WASD + mouse look)
- Point lights with phong shading
- ImGui controls for terrain + camera + lights
- Basic mouse picking

---

## Visuals

### Terrain
![Terrain Detail](media/Screenshot%20from%202025-08-15%2023-07-42.png)

### UI Controls
![UI Interface](media/Screenshot%20from%202025-08-19%2017-34-49.png)

### Demos
<video src="media/Screencast%20from%2008-16-2025%2004:52:03%20AM.webm" controls="controls" style="max-width: 730px;"></video>  
*[View Terrain Generation Video](ReadmeImagesAndVideos/Screencast%20from%2008-16-2025%2004:52:03%20AM.webm)*  

<video src="media/Screencast%20from%2008-19-2025%2005:16:44%20PM.webm" controls="controls" style="max-width: 730px;"></video>  
*[View Lighting & Camera Demo](ReadmeImagesAndVideos/Screencast%20from%2008-19-2025%2005:16:44%20PM.webm)*  

---

## Build

### Requirements
- CMake 3.6+
- OpenGL 3.3+
- Git

Dependencies are bundled (GLFW, GLAD, GLM, ImGui, stb).

```bash
git clone https://github.com/BlacckMangoo/ProceduralTerrainGeneratorOpenGL.git
cd ProceduralTerrainGeneratorOpenGL
mkdir build && cd build
cmake ..
make
./mygame

```
