cmake_minimum_required(VERSION 3.16)
project(solar_system_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/headers)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Collect source files
file(GLOB SOURCES
        "src/*.cpp"
        "src/*.c"
)

# Collect header files
file(GLOB HEADERS
        "headers/*.h"
        "headers/*.hpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
)

# Windows specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
            gdi32
            user32
            kernel32
            shell32
    )
endif()

# Copy shaders and textures to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Set working directory for debugging in CLion
set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)