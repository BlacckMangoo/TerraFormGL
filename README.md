
# Procedural Terrain Generator (OpenGL)

A real-time procedural terrain generator with dynamic lighting, built using modern OpenGL. This application demonstrates procedural terrain generation using noise algorithms, real-time rendering with modern OpenGL, and an interactive UI for parameter control.



## Features

- **Procedural Terrain Generation**: Create dynamic terrain landscapes using FastNoiseLite algorithm
- **Multiple Rendering Modes**: Switch between wireframe, solid, and point cloud visualization modes
- **Advanced Camera System**: First-person camera with smooth movement and adjustable parameters
- **Dynamic Lighting System**: Multiple colored point lights with proper attenuation and phong shading
- **Real-time Parameter Tuning**: Adjust terrain properties (frequency, amplitude, scale) on-the-fly
- **Ray Casting**: Select and interact with scene objects through mouse picking
- **ImGui Integration**: Comprehensive UI for parameter controls and real-time feedback
- **Resource Management**: Efficient shader and texture resource management system

## Technical Details

- Written in modern C++ (C++17) with OpenGL 3.3 Core Profile
- Uses GLSL shaders for lighting, terrain shading, and visual effects
- Implements efficient mesh generation algorithms with optimized vertex data structures
- Hardware-accelerated rendering with performance optimizations
- Modular architecture with clean separation of rendering, generation, and UI components

## Getting Started

### Prerequisites

- CMake 3.6+
- OpenGL 3.3+ compatible GPU
- Git (for cloning the repository)

All other dependencies are included in the repository:
- GLFW 3.3.2 (window management)
- GLAD (OpenGL loader)
- GLM (mathematics library)
- Dear ImGui (user interface)
- STB Image & STB TrueType (asset loading)

### Building

```bash
# Clone the repository
git clone https://github.com/BlacckMangoo/ProceduralTerrainGeneratorOpenGL.git
cd ProceduralTerrainGeneratorOpenGL

# Create build directory and compile
mkdir build && cd build
cmake ..
make
```

### Running

```bash
./mygame
```

## Controls

- **WASD**: Move camera position
- **Mouse Movement**: Look around/rotate camera
- **Mouse Wheel**: Zoom in/out
- **Left Click**: Select objects in the scene
- **UI Panels**: Adjust terrain and camera parameters in real-time
  - Terrain: Frequency, amplitude, scale, resolution
  - Camera: Movement speed, sensitivity, field of view
  - Lighting: Position, color, intensity of scene lights

## Project Structure

- `src/`: Core application logic and implementation files
- `include/`: Header files defining classes and interfaces
- `resources/shaders/`: GLSL shader programs for different rendering techniques
- `thirdparty/`: External libraries and dependencies
  - `glad/`: OpenGL loader
  - `glfw-3.3.2/`: Window management
  - `glm/`: Mathematics library
  - `imgui-docking/`: UI framework
  - `stb_image/` & `stb_truetype/`: Asset loading utilities

## Key Components

- **TerrainGenerator**: Creates procedural terrain meshes using noise algorithms
- **TerrainRenderer**: Handles rendering of terrain with proper lighting
- **Camera**: Manages view and projection matrices for scene rendering
- **ResourceManager**: Handles loading and caching of shader programs and textures
- **Light**: Represents point lights in the scene with proper attenuation models
- **App**: Core application class that orchestrates the rendering pipeline

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [FastNoiseLite](https://github.com/Auburn/FastNoiseLite) for efficient noise generation
- [OpenGL Tutorials](https://learnopengl.com/) for lighting and shader techniques
- [Dear ImGui](https://github.com/ocornut/imgui) for the user interface framework

---

Contributions and feedback are welcome! Feel free to submit issues or pull requests.
