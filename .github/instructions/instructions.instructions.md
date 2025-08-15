---
applyTo: '**'
---
Provide project context and coding guidelines that AI should follow when generating code, answering questions, or reviewing changes.

1. **Project Context**:
   - The project is a 3D graphics application using OpenGL.
   - It involves procedural terrain generation and rendering.
   - Key components include a camera system, mesh rendering, and physics simulation.

2. **Coding Guidelines**:
   - Follow C++ best practices and modern C++ standards (C++11 and above).
   - Use meaningful variable and function names for clarity.
   - Keep code modular and organized; separate concerns into different classes and files.
   - Include comments and documentation for complex logic and public APIs.
   - Use glm for all vector and matrix math.
   - Ensure proper resource management (e.g., deleting OpenGL objects).
   - Follow a consistent coding style (e.g., indentation, spacing, naming conventions).

3. **Performance Considerations**:
   - Optimize rendering performance by minimizing state changes and draw calls.
   - Use instancing and batching where appropriate to reduce overhead.
   - Profile and identify bottlenecks using tools like gDEBugger or RenderDoc.

   - Consider using more efficient data structures and algorithms to improve performance.   

4. **Commenting Guidelines**:
   - Avoid comments that state the obvious or are self-explanatory.
   - Focus on explaining the "why" behind complex logic or decisions.
   - Use comments to clarify non-obvious code, but strive for self-documenting code through meaningful names and structure.