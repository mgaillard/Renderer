add_library(RendererLib STATIC)

message(STATUS "Creating target 'RendererLib'")

if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(RendererLib PRIVATE /W4)
    target_compile_options(RendererLib PRIVATE -openmp:experimental)
    target_compile_options(RendererLib PRIVATE /arch:AVX2)
else()
    # lots of warnings and all warnings as errors
    target_compile_options(RendererLib PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(RendererLib PRIVATE -march=native)
endif()

set(HEADER_FILES
        include/AABB.h
        include/Camera.h
        include/Constants.h
        include/Dielectric.h
        include/FloatImage.h
        include/ImageConversion.h
        include/Lambertian.h
        include/Material.h
        include/MathUtils.h
        include/Mesh.h
        include/Metal.h
        include/Random.h
        include/Ray.h
        include/Renderer.h
        include/Scene.h
        include/Types.h
)

set(SRC_FILES
        src/AABB.cpp
        src/Camera.cpp
        src/Dielectric.cpp
        src/FloatImage.cpp
        src/ImageConversion.cpp
        src/Lambertian.cpp
        src/MathUtils.cpp
        src/Mesh.cpp
        src/Metal.cpp
        src/Random.cpp
        src/Renderer.cpp
        src/Scene.cpp
)

source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SRC_FILES})


target_sources(RendererLib
    PUBLIC
    ${HEADER_FILES}
    PRIVATE
    ${SRC_FILES}
)

target_include_directories(RendererLib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(RendererLib 
    PUBLIC
    glm::glm
    OpenMP::OpenMP_CXX
)
