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

# Create executable FIRST
add_executable(${PROJECT_NAME} ${SOURCES})

# THEN add include directories (moved here, after add_executable)
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
    # Disable some MSVC warnings if needed
    # target_compile_options(${PROJECT_NAME} PRIVATE /wd4996) # Disable deprecated warnings
else()
    # Enable address sanitizer for debug builds on GCC/Clang
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
        target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
    endif()
endif()

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
if(MSVC)
    message(STATUS "MSVC version: ${MSVC_VERSION}")
endif()