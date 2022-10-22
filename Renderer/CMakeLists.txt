add_executable(Renderer)

message(STATUS "Creating target 'Renderer'")

if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(Renderer PRIVATE /W4)
    target_compile_options(Renderer PRIVATE -openmp:experimental)
    target_compile_options(Renderer PRIVATE /arch:AVX2)
else()
    # lots of warnings and all warnings as errors
    target_compile_options(Renderer PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(Renderer PRIVATE -march=native)
endif()

set(HEADER_FILES )

set(SRC_FILES
        main.cpp
)

source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SRC_FILES})

# Copy the ReSharper config file
configure_file(
    "${CMAKE_SOURCE_DIR}/Renderer.sln.DotSettings"
    ${CMAKE_BINARY_DIR} COPYONLY
)

target_sources(Renderer
    PUBLIC
    ${HEADER_FILES}
    PRIVATE
    ${SRC_FILES}
)

target_link_libraries(Renderer 
    PUBLIC
    glm::glm
    OpenMP::OpenMP_CXX
    RendererLib
)
