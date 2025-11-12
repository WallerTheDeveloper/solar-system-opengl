cmake_minimum_required(VERSION 3.16)
project(solar_system_opengl VERSION 1.0.0 LANGUAGES CXX C)

#==============================================================================
# Project Configuration
#==============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "==================================================")
message(STATUS "Solar System OpenGL - Build Configuration")
message(STATUS "==================================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "==================================================")

#==============================================================================
# Compiler Flags
#==============================================================================

if(MSVC)
    # MSVC-specific flags
    add_compile_options(
            /W3                 # Warning level 3
            /permissive-        # Standards conformance
            /Zc:__cplusplus     # Enable __cplusplus macro
    )

    # Debug/Release specific flags
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /D_DEBUG /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG /MD")

    # Enable debug heap in debug mode
    add_compile_definitions($<$<CONFIG:Debug>:_CRTDBG_MAP_ALLOC>)

    message(STATUS "MSVC version: ${MSVC_VERSION}")
else()
    # GCC/Clang flags
    add_compile_options(
            -Wall               # Enable warnings
            -Wextra             # Extra warnings
            -Wpedantic          # Pedantic warnings
            $<$<CONFIG:Debug>:-g>           # Debug symbols
            $<$<CONFIG:Debug>:-O0>          # No optimization in debug
            $<$<CONFIG:Release>:-O3>        # Full optimization in release
    )

    # Address sanitizer for debug builds (optional, uncomment if needed)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND ENABLE_ASAN)
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
        message(STATUS "Address Sanitizer: ENABLED")
    endif()
endif()

#==============================================================================
# Find Dependencies
#==============================================================================

# OpenGL (required)
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL: Found")

#==============================================================================
# Source Files
#==============================================================================

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.c"
)

# Collect all header files
file(GLOB_RECURSE HEADER_FILES
        "${CMAKE_SOURCE_DIR}/src/*.h"
        "${CMAKE_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_SOURCE_DIR}/include/*.h"
        "${CMAKE_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/headers/*.h"
        "${CMAKE_SOURCE_DIR}/headers/*.hpp"
)

# Organize sources by category for better IDE experience
set(CORE_SOURCES
        src/core/Engine.cpp
        src/core/Shader.cpp
        src/core/Camera.cpp
)

set(UTILS_SOURCES
        src/utils/ResourcePath.cpp          # Add ResourcePath here
        src/utils/debug_utils.cpp
)

set(AUDIO_SOURCES
        src/core/audio/AudioManager.cpp
        src/miniaudio_impl.cpp              # miniaudio implementation
)

set(RENDERING_SOURCES
        src/rendering/renderers/SceneRenderer.cpp
        src/rendering/renderers/UIRenderer.cpp
        src/rendering/renderables/scene/CelestialBody.cpp
        src/rendering/renderables/scene/Skybox.cpp
        src/rendering/RenderContext.cpp
        src/rendering/text/TextRenderer.cpp
)

set(GRAPHICS_SOURCES
        src/graphics/buffer/BufferManager.cpp
        src/core/texturing/TextureManager.cpp
)

set(INPUT_SOURCES
        src/core/input/InputManager.cpp
)

set(WINDOW_SOURCES
        src/core/window/WindowManager.cpp
)

set(CELESTIAL_SOURCES
        src/celestialbody/CelestialBodyPicker.cpp
)

# GLAD loader (add if not already collected)
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src/glad.c")
    set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/include/glad/glad.c")
else()
    set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/src/glad.c")
endif()

# Combine all sources
set(ALL_SOURCES
        ${SOURCE_FILES}
        ${GLAD_SOURCE}
)

# Print source count
list(LENGTH ALL_SOURCES SOURCE_COUNT)
message(STATUS "Source files found: ${SOURCE_COUNT}")

#==============================================================================
# Create Executable
#==============================================================================

add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${HEADER_FILES})

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "SolarSystem"
        DEBUG_POSTFIX "_d"
)

#==============================================================================
# Include Directories
#==============================================================================

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/headers
)

message(STATUS "Include directories configured")

#==============================================================================
# Platform-Specific Configuration
#==============================================================================

if(WIN32)
    #--------------------------------------------------------------------------
    # Windows Configuration
    #--------------------------------------------------------------------------
    message(STATUS "Configuring for Windows...")

    # Try to find local GLFW library first
    if(EXISTS "${CMAKE_SOURCE_DIR}/lib/glfw3.lib")
        message(STATUS "Using local GLFW: ${CMAKE_SOURCE_DIR}/lib/glfw3.lib")
        target_link_libraries(${PROJECT_NAME} PRIVATE
                OpenGL::GL
                ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
                gdi32
                user32
                kernel32
                shell32
        )
    else()
        # Try to find installed GLFW
        find_package(glfw3 REQUIRED)
        message(STATUS "Using system GLFW")
        target_link_libraries(${PROJECT_NAME} PRIVATE
                OpenGL::GL
                glfw
                gdi32
                user32
                kernel32
                shell32
        )
    endif()

    # Windows-specific compile definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            NOMINMAX                # Prevent min/max macro conflicts
            WIN32_LEAN_AND_MEAN     # Reduce Windows header size
            _CRT_SECURE_NO_WARNINGS # Disable CRT security warnings
    )

    # Set subsystem to CONSOLE (can be changed to WINDOWS for release)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE FALSE  # Set to TRUE for release without console
    )

elseif(APPLE)
    #--------------------------------------------------------------------------
    # macOS Configuration
    #--------------------------------------------------------------------------
    message(STATUS "Configuring for macOS...")

    # Find GLFW
    find_package(glfw3 REQUIRED)

    # Find macOS frameworks
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
            OpenGL::GL
            glfw
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
    )

    # macOS-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE FALSE                 # Set to TRUE for .app bundle
            XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17"
            XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
    )

    message(STATUS "macOS frameworks configured")

else()
    #--------------------------------------------------------------------------
    # Linux Configuration
    #--------------------------------------------------------------------------
    message(STATUS "Configuring for Linux...")

    # Find GLFW
    find_package(glfw3 REQUIRED)

    # Find required packages
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
            OpenGL::GL
            glfw
            ${CMAKE_DL_LIBS}
            Threads::Threads
            X11::X11
    )

    message(STATUS "Linux libraries configured")
endif()

#==============================================================================
# Working Directory & Resource Paths
#==============================================================================

# Set working directory for Visual Studio
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    message(STATUS "VS debugger working directory: ${CMAKE_SOURCE_DIR}")
endif()

# Set working directory for Xcode
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

#==============================================================================
# Resource Handling
#==============================================================================

# Option to copy resources to build directory
option(COPY_RESOURCES "Copy resource files to build directory" OFF)

if(COPY_RESOURCES)
    message(STATUS "Resources will be copied to build directory")

    # Create resource directories in build
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/audio)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/texture)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/shaders)

    # Copy resources
    file(GLOB_RECURSE AUDIO_FILES "${CMAKE_SOURCE_DIR}/audio/*")
    file(GLOB_RECURSE TEXTURE_FILES "${CMAKE_SOURCE_DIR}/texture/*")
    file(GLOB_RECURSE SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*")

    foreach(FILE ${AUDIO_FILES})
        file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/audio" ${FILE})
        configure_file(${FILE} ${CMAKE_BINARY_DIR}/bin/audio/${REL_PATH} COPYONLY)
    endforeach()

    foreach(FILE ${TEXTURE_FILES})
        file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/texture" ${FILE})
        configure_file(${FILE} ${CMAKE_BINARY_DIR}/bin/texture/${REL_PATH} COPYONLY)
    endforeach()

    foreach(FILE ${SHADER_FILES})
        file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/shaders" ${FILE})
        configure_file(${FILE} ${CMAKE_BINARY_DIR}/bin/shaders/${REL_PATH} COPYONLY)
    endforeach()

    # Update resource path initialization in code to use "./" instead of "../"
    message(STATUS "Remember to set ResourcePath::initialize(\"./\") in your main.cpp")
else()
    message(STATUS "Resources will be loaded from source directory")
    message(STATUS "Remember to set ResourcePath::initialize(\"../\") in your main.cpp")
endif()

#==============================================================================
# IDE Support (Source Grouping)
#==============================================================================

# Group sources by directory for better IDE organization
source_group("Source Files\\Core" FILES ${CORE_SOURCES})
source_group("Source Files\\Utils" FILES ${UTILS_SOURCES})
source_group("Source Files\\Audio" FILES ${AUDIO_SOURCES})
source_group("Source Files\\Rendering" FILES ${RENDERING_SOURCES})
source_group("Source Files\\Graphics" FILES ${GRAPHICS_SOURCES})
source_group("Source Files\\Input" FILES ${INPUT_SOURCES})
source_group("Source Files\\Window" FILES ${WINDOW_SOURCES})
source_group("Source Files\\Celestial" FILES ${CELESTIAL_SOURCES})

#==============================================================================
# Installation (Optional)
#==============================================================================

# Install executable
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

# Install resources
install(DIRECTORY
        ${CMAKE_SOURCE_DIR}/audio
        ${CMAKE_SOURCE_DIR}/texture
        ${CMAKE_SOURCE_DIR}/shaders
        DESTINATION bin
)

#==============================================================================
# Print Configuration Summary
#==============================================================================

message(STATUS "==================================================")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Executable: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SolarSystem")
message(STATUS "  Copy resources: ${COPY_RESOURCES}")
message(STATUS "==================================================")

#==============================================================================
# Custom Targets
#==============================================================================

# Add a custom target to show all project files in IDE
add_custom_target(ProjectFiles SOURCES ${HEADER_FILES})

# Add a target to clean and rebuild
add_custom_target(rebuild
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Cleaning and rebuilding project..."
)