# Solar System Simulation - Custom Graphics Engine

A physics-accurate 3D solar system simulation built from scratch using C++17 and OpenGL 3.3. This project demonstrates a **custom rendering engine** with clean architecture, modular design, and extensible systems.

![C++](https://img.shields.io/badge/C++-17-00599C.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)

---

## 🎥 Video Demo

[![Solar System Simulation Demo](https://img.youtube.com/vi/-Bvyj-GJXLw/maxresdefault.jpg)](https://www.youtube.com/watch?v=-Bvyj-GJXLw)

**[Download and Play](https://wallerthedeveloper.itch.io/solar-system-simulation)**

---

## 🎯 Overview

This project showcases a custom graphics engine architecture built for real-time 3D rendering. While the solar system simulation demonstrates orbital mechanics using Kepler's laws, the underlying engine is designed to be extended into different 3D applications.

**Core Focus:** Professional C++ architecture, graphics programming fundamentals, and scalable system design.

---

## 🏗️ Architecture

### System Layers

The engine follows a **layered architecture** with clear separation between systems:

```
Application Layer (SolarSystemApp)
        ↓
Engine Core (Engine, EngineContext, Managers)
        ↓
Graphics Systems (Rendering, Buffers, Shaders, Meshes)
        ↓
Platform Layer (GLFW, OpenGL, GLM)
```

Each layer depends only on layers below it, preventing circular dependencies and maintaining modularity.

### Core Engine Systems

#### **1. Engine Core** (`src/core/Engine.h`)
Central orchestration of all subsystems with a fixed-timestep game loop.

**Key responsibilities:**
- Main loop management (62.5 tick/sec logic, independent render rate)
- System lifecycle coordination
- Frame timing and FPS tracking
- Callback-driven update and render cycles

**Why this matters:** Decouples physics simulation from rendering, ensuring consistent behavior across different hardware.

#### **2. Buffer Management System** (`src/graphics/buffer/BufferManager.h`)
Centralized OpenGL buffer management with RAII principles and automatic resource tracking.

**Features:**
- Unified interface for VAO/VBO/EBO creation
- VRAM usage monitoring and reporting
- Named buffer ownership for debugging
- Automatic cleanup through `BufferHandle` wrapper (move-only semantics)

**Architecture benefit:** Single point of truth for all GPU memory allocations. Resources can't leak—handles guarantee cleanup on destruction.

#### **3. Rendering Pipeline** (`src/rendering/`)
Modular rendering system with separated concerns:

- **SceneRenderer**: 3D scene rendering with proper depth testing and state management
- **UIRenderer**: 2D overlay rendering for controls and info panels
- **TextRenderer**: Real-time text rendering using FreeType and orthographic projection

**Rendering order:**
1. Skybox (modified depth test for background)
2. Opaque objects (front-to-back for early depth rejection)
3. Transparent objects (back-to-front with alpha blending)

#### **4. Shader System** (`src/core/Shader.h`)
Type-safe shader program wrapper with compile-time error checking and uniform setters.

**Design choice:** Each renderable manages its own shader instance rather than using a global shader pool. This trades some memory for flexibility—objects can customize shaders without affecting others.

#### **5. Resource Managers**
Specialized managers for different resource types:

- **TextureManager**: Texture loading (STB Image integration), cubemap creation
- **MeshGenerator**: Procedural geometry (spheres, boxes, parametric control)
- **WindowManager**: GLFW window and context management
- **InputManager**: Event-driven input with GLFW callback forwarding
- **AudioManager**: Background music playback using miniaudio library. Plays a drum and bass track during simulation to enhance the space exploration atmosphere.

**Pattern:** Manager objects own resources and provide factory methods. Resources are referenced by ID/handle, never by pointer.

#### **6. Physics Simulation** (`src/celestialbody/`)
Orbital mechanics implementation using Kepler's laws with real astronomical data (semi-major axis, eccentricity, orbital period).

**Implementation:** Fixed timestep updates ensure physics determinism independent of frame rate. Angular velocity calculated from orbital period drives position updates.

---

## 🧩 Design Patterns & Principles

### Dependency Injection
Systems receive their dependencies through constructor injection:
```cpp
CelestialBody(BufferManager& bufferManager,
              MeshGenerator& meshGenerator,
              TextureManager& textureManager);
```
**Benefit:** Testable, loosely coupled, dependencies explicit at construction.

### RAII Resource Management
All resources wrapped in RAII objects (smart pointers, custom handles):
```cpp
class BufferHandle {
    // Non-copyable, moveable
    ~BufferHandle(); // Automatic cleanup
};
```
**Benefit:** Exception-safe, no manual cleanup, impossible to leak resources.

### Interface-Based Design
Common interface for all scene objects:
```cpp
class ISceneRenderable {
    virtual void render(glm::mat4 model, glm::mat4 view, 
                       glm::mat4 projection) const = 0;
    virtual void update(float deltaTime) {}
};
```
**Benefit:** Renderer doesn't know concrete types. New renderables integrate without modifying existing code.

### Separation of Concerns
- **Core layer**: Engine loop, system management
- **Graphics layer**: Rendering, buffers, shaders
- **Domain layer**: Physics, celestial body logic
- **Application layer**: User-facing logic, UI

Clear boundaries prevent coupling between unrelated systems.

---

## 🔧 Technical Implementation

### Modern C++ (C++17)
- Smart pointers for ownership semantics
- Move semantics for efficient resource transfer
- Structured bindings for cleaner iteration
- RAII throughout for automatic resource management
- `constexpr` for compile-time computation

### Graphics Programming (OpenGL 3.3)
- VAO/VBO/EBO for efficient rendering
- Custom shader pipeline with GLSL
- Texture mapping and cubemap skyboxes
- Alpha blending for transparency
- Depth testing and state management
- Real-time text rendering with FreeType

### Build System (CMake)
- Cross-platform configuration
- Platform-specific compiler flags
- Warning level enforcement (`-Wall -Wextra -pedantic` / `/W3`)
- Debug sanitizers (AddressSanitizer on GCC/Clang)
- Automatic resource copying to build directory

---

## ⚡ Performance Considerations

### Buffer Optimization
- Batch buffer creation minimizes GPU state changes
- Static draw hints for GPU optimization
- Vertex data uploaded once, reused via handles

### State Management
- State changes minimized in render loop
- Objects sorted by shader/texture when possible
- Render state cached to avoid redundant calls

### Memory Tracking
Real-time VRAM monitoring for profiling:
```cpp
void BufferManager::printActiveBuffers() const;
size_t getTotalVRAMUsage() const;
```

### Frame Independence
Delta time ensures physics behaves identically on all hardware:
```cpp
void update(float deltaTime) {
    position += velocity * deltaTime;
}
```

---

## 🎮 Extensibility

The architecture supports extension in multiple directions:

### Adding New Renderable Types
The `ISceneRenderable` interface makes it trivial to add new 3D objects to the scene. Any class implementing this interface can be rendered by the existing rendering pipeline without modifications.

**Examples of new renderables that could be added:**

```cpp
// Asteroid - small rocky object with custom mesh
class Asteroid : public ISceneRenderable {
public:
    Asteroid(BufferManager& bufferManager,
             MeshGenerator& meshGenerator,
             TextureManager& textureManager);
    
    void render(glm::mat4 model, glm::mat4 view,
               glm::mat4 projection) const override;
    void update(float deltaTime) override;
};

// Spaceship - player-controlled vehicle with animation
class Spaceship : public ISceneRenderable {
public:
    Spaceship(BufferManager& bufferManager,
              TextureManager& textureManager);
    
    void render(glm::mat4 model, glm::mat4 view,
               glm::mat4 projection) const override;
    void update(float deltaTime) override;
    
    void setThrust(float amount);
    void rotate(glm::vec3 axis, float angle);
};

// ParticleEmitter - engine effects, explosions
class ParticleEmitter : public ISceneRenderable {
public:
    ParticleEmitter(BufferManager& bufferManager,
                    size_t maxParticles);
    
    void render(glm::mat4 model, glm::mat4 view,
               glm::mat4 projection) const override;
    void update(float deltaTime) override;
    
    void emit(glm::vec3 position, glm::vec3 velocity);
};

// Terrain - height-mapped ground with LOD
class Terrain : public ISceneRenderable {
public:
    Terrain(BufferManager& bufferManager,
            const std::string& heightmapPath);
    
    void render(glm::mat4 model, glm::mat4 view,
               glm::mat4 projection) const override;
    
    float getHeightAt(float x, float z) const;
};
```

Each new type would:
1. Implement `ISceneRenderable` interface
2. Use existing `BufferManager` and `Shader` systems
3. Integrate automatically with the rendering pipeline
4. Require no changes to core engine code

### Different Application Domains
The rendering engine is application-agnostic:
- **Data visualization**: Replace celestial bodies with data points for 3D charts
- **Architectural visualization**: Use mesh importers instead of procedural generation
- **Game development**: Add gameplay systems using existing engine core
- **Scientific simulation**: Swap physics system while keeping rendering intact

### Full Game Engine Evolution
Manager pattern and layered architecture support adding:
- Entity-Component-System (ECS)
- Scene graph hierarchy
- Serialization system
- Asset pipeline and hot-reloading

---

## 🛠️ Building

### Prerequisites
```bash
# Windows
- Visual Studio 2019+ or MinGW
- CMake 3.16+
```

### Build Steps
```bash
git clone <repository-url>
cd solar-system-simulation
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Run
```bash
.\bin\Release\solar_system_opengl.exe
```

---

## 📁 Project Structure

```
src/
├── core/                             # Engine core systems (Engine, Shader, EngineContext)
│   ├── audio/                        # Audio playback system with miniaudio
│   ├── input/                        # Keyboard/mouse input handling with GLFW callbacks
│   ├── texturing/                    # Texture loading and management with STB Image
│   └── window/                       # GLFW window creation and OpenGL context management
│
├── graphics/                         # Graphics layer systems
│   ├── buffer/                       # GPU buffer management (VAO/VBO/EBO) with RAII wrappers
│   └── mesh/                         # Procedural geometry generation (spheres, boxes, primitives)
│
├── rendering/                        # Rendering pipeline
│   ├── renderers/                    # Specialized renderers (Scene, UI, Text with FreeType)
│   └── renderables/                  # Renderable object implementations
│       └── scene/                    # 3D scene objects (CelestialBody, Skybox, ISceneRenderable)
│
├── celestialbody/                    # Domain-specific logic
│                                     # Planet factory, ray casting picker, data structures
│
├── utils/                            # Utility functions (debug macros, math helpers)
│
└── SolarSystemApp.h/cpp + main.cpp   # Application entry point

shaders/                              # GLSL shader programs (vertex/fragment)
textures/                             # Planet textures (NASA sources) and skybox cubemap
audio/                                # Background music (dnb.mp3)
include/                              # Third-party headers (GLFW, GLM, GLAD, STB Image)
CMakeLists.txt                        # Cross-platform build configuration
```

---

## 📊 Technical Metrics

- **12 independent modules** with clear interfaces
- **4 architectural layers** with unidirectional dependencies
- **60+ FPS** on integrated graphics
- **<50 MB** typical VRAM usage
- **<30 seconds** clean build time

---

## 🎓 Demonstrated Skills

**C++ Expertise:**
- Modern C++17 features and best practices
- Memory management and RAII patterns
- Template metaprogramming for compile-time optimization
- Move semantics and perfect forwarding

**Graphics Programming:**
- OpenGL 3.3 core profile from scratch
- Vertex pipeline and shader programming
- 3D mathematics (matrices, vectors, transformations)
- Rendering techniques (alpha blending, depth testing, cubemaps)

**Software Architecture:**
- Layered architecture with dependency inversion
- Design patterns (Factory, Manager, RAII, Dependency Injection)
- Interface-based design for extensibility
- Separation of concerns across modules

**System Design:**
- Resource management at scale
- Performance profiling and optimization
- Cross-platform build systems (CMake)
- Debugging and error handling strategies

---

## 🙏 Acknowledgments

- **Textures**: NASA and public domain resources
- **Learning Resources**: learnopengl.com, Khronos documentation
- **Libraries**: GLFW, GLM, GLAD, STB Image, miniaudio

---

## 👤 Author

**Danylo Golosov**  
Portfolio: [wallerthedeveloper.itch.io](https://wallerthedeveloper.itch.io/solar-system-simulation)

---
