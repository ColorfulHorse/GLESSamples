//
// Created by admin on 2022/10/8.
//

#ifndef GLSAMPLE_SHADER_H
#define GLSAMPLE_SHADER_H

#include <fstream>
#include <GLES3/gl32.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "AssetUtil.h"

class Shader {
public:
    unsigned int id;
    Shader() {
    }
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr) {
        // 1. retrieve the vertex/fragment source code from filePath
        const char* vertexCode = AssetUtil::loadTextAsset(vertexPath);
        const char* fragmentCode = AssetUtil::loadTextAsset(fragmentPath);
        const char* geometryCode = nullptr;
        if (geometryPath != nullptr) {
            geometryCode = AssetUtil::loadTextAsset(geometryPath);
        }
        unsigned int vertex, fragment, geometry;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        if (geometryPath != nullptr) {
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &geometryCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        if (geometryPath != nullptr)
            glAttachShader(id, geometry);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void use() {
        glUseProgram(id);
    }

    void release() {
        glDeleteProgram(id);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
    }


    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
    }
    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }

    void bindUniformBlock(const std::string &name, GLuint bindingIndex) {
        // 获取uniform块索引
        unsigned int index = glGetUniformBlockIndex(id, name.c_str());
        std::cout << "uniform block index:" << index << std::endl;
        glUniformBlockBinding(id, index, bindingIndex);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, const char* type) {
        int success;
        char infoLog[1024];
        if (strcmp(type, "PROGRAM") != 0) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                LOGE("Shader", "ERROR::SHADER_COMPILATION_ERROR of type: %s \n %s", type, infoLog);
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                LOGE("Shader", "ERROR::SHADER_LINKING_ERROR of type: %s \n %s", type, infoLog);
            }
        }
    }
};

#endif //GLSAMPLE_SHADER_H
