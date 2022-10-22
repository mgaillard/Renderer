add_executable(RendererMPI)

message(STATUS "Creating target 'RendererMPI'")

if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(RendererMPI PRIVATE /W4)
    target_compile_options(RendererMPI PRIVATE -openmp:experimental)
    target_compile_options(RendererMPI PRIVATE /arch:AVX2)
else()
    # lots of warnings and all warnings as errors
    target_compile_options(RendererMPI PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(RendererMPI PRIVATE -march=native)
endif()

set(HEADER_FILES )

set(SRC_FILES
        main.cpp
)

source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SRC_FILES})

target_sources(RendererMPI
    PUBLIC
    ${HEADER_FILES}
    PRIVATE
    ${SRC_FILES}
)

target_link_libraries(RendererMPI 
    PUBLIC
    glm::glm
    OpenMP::OpenMP_CXX
    MPI::MPI_CXX
    RendererLib
)
