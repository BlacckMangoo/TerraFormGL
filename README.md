
# Procedural Terrain Generator (OpenGL)

A real-time procedural terrain generator with dynamic lighting, built using modern OpenGL.

<img width="1410" height="842" alt="Screenshot From 2025-07-09 12-20-46" src="https://github.com/user-attachments/assets/1b87b0d8-101c-4e1e-b17d-7c3199fb1fcd" />

## Features

- **Dynamic Terrain Generation**: Create procedural terrain landscapes with customizable parameters
- **Multiple Rendering Modes**: Switch between wireframe, solid, and point cloud views
- **Interactive Camera**: Full first-person camera controls with smooth movement
- **Dynamic Lighting System**: Multiple colored point lights with proper attenuation
- **Real-time Parameter Tuning**: Adjust terrain properties on-the-fly
- **Ray Casting**: Select and interact with scene objects
- **ImGui Integration**: Clean user interface for parameter controls

## Technical Details

- Written in C++ with modern OpenGL
- Uses GLSL shaders for lighting and terrain rendering
- Implements efficient mesh generation algorithms
- Hardware-accelerated rendering for smooth performance

## Getting Started

### Prerequisites

- CMake 3.10+
- OpenGL 4.3+ compatible GPU
- GLFW, GLAD, GLM libraries

### Building

```bash
mkdir build && cd build
cmake ..
make
```

### Running

```bash
./mygame
```

## Controls

- **WASD**: Move camera
- **Mouse**: Look around
- **Mouse Wheel**: Zoom in/out
- **Left Click**: Select objects
- **UI Panel**: Adjust terrain parameters and rendering modes

## Project Structure

- `src`: Core application logic
- `include`: Header files
- `resources`: Shaders and textures
- `thirdparty`: External dependencies

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [OpenGL Tutorials](https://learnopengl.com/) for lighting and shader techniques
- [Noise algorithms](https://mrl.cs.nyu.edu/~perlin/noise/) for terrain generation

---

Contributions and feedback are welcome!
