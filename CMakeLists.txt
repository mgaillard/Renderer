cmake_minimum_required(VERSION 3.16.0)

project(Renderer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activate OpenMP
find_package(OpenMP REQUIRED)

# Try to find MPI
find_package(MPI)

# Add CMake recipes
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# Dependencies for application
include(glm)

add_subdirectory(RendererLib)
add_subdirectory(Renderer)
if (MPI_FOUND)
    add_subdirectory(RendererMPI)
endif()

# Set the project as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Renderer)
