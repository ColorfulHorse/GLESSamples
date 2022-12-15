#include "GLUtils.h"
#include "core/AssetUtil.h"

 GLuint GLUtils::loadTextureFromPath(const char *path, GLint repeatMode, bool gamma) {
	int32_t width, height, channels;
	stbi_uc *data = stbi_load(path, &width, &height, &channels, 0);
	return loadTexture(path, width, height, channels, data, repeatMode, gamma);
 }

GLuint GLUtils::loadAssetsTexture(const char *path, GLint repeatMode, bool gamma) {
	long len;
	unsigned char* data = AssetUtil::loadAsset(path, &len);
	int32_t width, height, channels;
	stbi_uc *imgData = stbi_load_from_memory(data, len, &width, &height, &channels, 0);
	return loadTexture(path, width, height, channels, imgData, repeatMode, gamma);
}

GLuint GLUtils::loadAssetsTexture(const char *path, int *width, int *height, GLint repeatMode, bool gamma) {
	long len;
	unsigned char* data = AssetUtil::loadAsset(path, &len);
	int32_t channels;
	stbi_uc *imgData = stbi_load_from_memory(data, len, width, height, &channels, 0);
	return loadTexture(path, *width, *height, channels, imgData, repeatMode, gamma);
}

GLuint
GLUtils::loadTexture(const char *path,
					 int width, int height, int channels, stbi_uc *data, GLint repeatMode, bool gamma) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	GLenum internalFormat;
	GLenum dataFormat;

	if (channels == 1) {
		internalFormat = GL_R8;
		dataFormat = GL_RED;
	} else if (channels == 3) {
		dataFormat = GL_RGB;
		// SRGB8不支持render mipmap
		// https://stackoverflow.com/questions/70285879/android12-opengles3-0-glteximage2d-0x502-error/70286532#70286532
		// https://stackoverflow.com/questions/43967363/glgeneratemipmap-with-gl-srgb8-in-opengl-es-3-results-in-black-texture
		// Generating mipmaps on the GPU requires the color format to be both filterable (to create the downsamples) and renderable (to write the output image)
		// "texture-filterable" allowed me to use LINEAR min filter instead of NEAREST and being "color-renderable" allows me to generate the mipmap textures
		internalFormat = gamma ? GL_SRGB8 : GL_RGB;
	} else if (channels == 4) {
		dataFormat = GL_RGBA;
		internalFormat = gamma ? GL_SRGB8_ALPHA8 : GL_RGBA;
	}

	// 纹理坐标超出边界时如何采样
	if (repeatMode != -1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	}
	// 放大缩小纹理时如何采样
	// GL_LINEAR_MIPMAP_LINEAR，对两个邻近层级的纹理做线性插值，并且采样时也使用线性插值
	// https://www.jianshu.com/p/6fce039f1ec3
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		// 根据图片创建纹理
		// format对照表https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glTexImage2D.xhtml
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		// 创建多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << path << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return textureId;
}

GLuint GLUtils::textureFromDict(const char *filename, const std::string &directory) {
	std::string path = directory + '/' + filename;
	return loadTextureFromPath(path.c_str(), GL_REPEAT);
}

GLuint GLUtils::loadCubeMap(std::vector<std::string> faces) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	GLenum format;
	int32_t width, height, channels;
	for (size_t i = 0; i < faces.size(); i++) {
		std::string path = faces[i];
		stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		if (data) {
			// 根据图片创建纹理
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		} else {
			std::cout << "Failed to load texture" << path << std::endl;
		}
		stbi_image_free(data);
	}
	// 环绕和过滤
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureId;
}

GLuint GLUtils::loadMemoryTexture(int width, int height, int channels, unsigned char *data,
								  GLint repeatMode, bool gamma) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	GLenum internalFormat;
	GLenum dataFormat;

	if (channels == 1) {
		internalFormat = GL_R8;
		dataFormat = GL_RED;
	} else if (channels == 3) {
		dataFormat = GL_RGB;
		// SRGB8不支持render mipmap
		// https://stackoverflow.com/questions/70285879/android12-opengles3-0-glteximage2d-0x502-error/70286532#70286532
		// https://stackoverflow.com/questions/43967363/glgeneratemipmap-with-gl-srgb8-in-opengl-es-3-results-in-black-texture
		// Generating mipmaps on the GPU requires the color format to be both filterable (to create the downsamples) and renderable (to write the output image)
		// "texture-filterable" allowed me to use LINEAR min filter instead of NEAREST and being "color-renderable" allows me to generate the mipmap textures
		internalFormat = gamma ? GL_SRGB8 : GL_RGB;
	} else if (channels == 4) {
		dataFormat = GL_RGBA;
		internalFormat = gamma ? GL_SRGB8_ALPHA8 : GL_RGBA;
	}

	// 纹理坐标超出边界时如何采样
	if (repeatMode != -1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	}
	// 放大缩小纹理时如何采样
	// GL_LINEAR_MIPMAP_LINEAR，对两个邻近层级的纹理做线性插值，并且采样时也使用线性插值
	// https://www.jianshu.com/p/6fce039f1ec3
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		// 根据图片创建纹理
		// format对照表https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glTexImage2D.xhtml
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		// 创建多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture from memory" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

