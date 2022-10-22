#pragma once

#include <filesystem>

#include "Scene.h"

/**
 * \brief Create a reference scene for testing the renderer
 * \param width Width in pixels of the rendering (for the camera)
 * \param height Height in pixels of the rendering (for the camera)
 * \param pathToMeshes Path to the folder containing the OBJ meshes
 * \return The reference scene
 */
std::unique_ptr<Scene> createReferenceScene(int width, int height, const std::filesystem::path& pathToMeshes);
