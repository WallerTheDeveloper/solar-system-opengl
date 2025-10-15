cmake_minimum_required(VERSION 3.16)
project(solar_system_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler-specific debug flags
if(MSVC)
    # MSVC debug flags
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
    # Enable more warnings
    add_compile_options(/W3)
else()
    # GCC/Clang debug flags
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    # Enable more warnings
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Collect source files
file(GLOB_RECURSE SOURCES
        "src/*.h"
        "src/*.cpp"
        "src/*.c"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/headers
)

# Compiler-specific settings
if(MSVC)
    # Enable Visual Studio debug heap in debug mode
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:_CRTDBG_MAP_ALLOC>
    )
else()
    # Enable address sanitizer for debug builds on GCC/Clang
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
        target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
    endif()
endif()

# Platform-specific GLFW and library linking
if(WIN32)
    # Windows: Use local glfw3.lib if it exists, otherwise try to find it
    if(EXISTS "${CMAKE_SOURCE_DIR}/lib/glfw3.lib")
        message(STATUS "Using local GLFW library: ${CMAKE_SOURCE_DIR}/lib/glfw3.lib")
        target_link_libraries(${PROJECT_NAME}
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
        target_link_libraries(${PROJECT_NAME}
                OpenGL::GL
                glfw
                gdi32
                user32
                kernel32
                shell32
        )
    endif()

elseif(APPLE)
    # macOS: Use system GLFW
    find_package(glfw3 REQUIRED)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(${PROJECT_NAME}
            OpenGL::GL
            glfw
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
    )

    # Optional: Create macOS app bundle
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE FALSE  # Set to TRUE if you want .app bundle
    )
endif()

# Copy resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Set working directory for debugging
set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
if(MSVC)
    message(STATUS "MSVC version: ${MSVC_VERSION}")
endif()