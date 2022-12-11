#ifndef UTILS_H
#define UTILS_H

#include <GLES3/gl32.h>
#include "stb_image.h"
#include <cstdint>
#include <iostream>
#include <vector>

class GLUtils {
public:
    static GLuint loadTextureFromPath(const char *path, GLint repeatMode = -1, bool gamma = false);

    static GLuint textureFromDict(const char *filename, const std::string &directory);

    static GLuint loadAssetsTexture(const char *path, GLint repeatMode = -1,
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
    static GLuint loadAssetsTexture(const char *path, int *width, int *height, GLint repeatMode = -1,
                                      bool gamma = false);

    static GLuint loadCubeMap(std::vector<std::string> faces);


    static GLuint
    loadTexture(const char *path,
                int width, int height, int channels, stbi_uc *data, GLint repeatMode = -1, bool gamma = false);
};

#endif