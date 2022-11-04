#ifndef UTILS_H
#define UTILS_H

#include <GLES3/gl32.h>
#include "stb_image.h"
#include <cstdint>
#include <iostream>
#include <vector>

class GLUtils {
public:
    static uint32_t loadTextureFromPath(const char *path, GLint repeatMode = -1, bool gamma = false);

    static uint32_t textureFromDict(const char *filename, const std::string &directory);

    static uint32_t loadAssetsTexture(const char *path, GLint repeatMode = -1,
                                      bool gamma = false);
    /**
     * 从assets加载纹理资源
     * @param path
     * @param width
     * @param height
     * @param repeatMode
     * @param gamma
     * @return
     */
    static uint32_t loadAssetsTexture(const char *path, int *width, int *height, GLint repeatMode = -1,
                                      bool gamma = false);

    static uint32_t loadCubeMap(std::vector<std::string> faces);


    static uint32_t
    loadTexture(const char *path, GLint repeatMode, bool gamma,
                int width, int height, int channels, stbi_uc *data);
};

#endif