cmake_minimum_required(VERSION 3.8)
project(Renderer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(SOURCE_FILES Renderer/main.cpp
			     Renderer/Camera.h Renderer/Camera.cpp
				 Renderer/Dielectric.h Renderer/Dielectric.cpp
				 Renderer/FloatImage.h Renderer/FloatImage.cpp
				 Renderer/ImageConversion.h Renderer/ImageConversion.cpp
				 Renderer/Lambertian.h Renderer/Lambertian.cpp
				 Renderer/Material.h
				 Renderer/MathUtils.h Renderer/MathUtils.cpp
				 Renderer/Mesh.h Renderer/Mesh.cpp
				 Renderer/Metal.h Renderer/Metal.cpp
				 Renderer/Random.h Renderer/Random.cpp
				 Renderer/Ray.h
				 Renderer/Renderer.h Renderer/Renderer.cpp
				 Renderer/Scene.h Renderer/Scene.cpp)

add_executable(Renderer ${SOURCE_FILES})
target_include_directories(Renderer PRIVATE Renderer)
target_include_directories(Renderer PRIVATE external)
