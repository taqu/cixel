#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "shader.h"

namespace
{
    GLint checkInfoLog(GLuint id, GLenum name)
    {
        GLint result;
        GLint infoLogLength;
        glGetShaderiv(id, name, &result);
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(0 < infoLogLength) {
            char* info = reinterpret_cast<char*>(malloc(infoLogLength + 1));
            glGetShaderInfoLog(id, infoLogLength, NULL, info);
            fprintf(stderr, "INFO:%s\n", info);
            free(info);
        }
        return result;
    }
}

GLuint createShaders(const char* vertexData, const char* fragmentData)
{
    // Create shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile shaders
    glShaderSource(vertexShaderID, 1, &vertexData, NULL);
    glCompileShader(vertexShaderID);
    checkInfoLog(vertexShaderID, GL_COMPILE_STATUS);

    glShaderSource(fragmentShaderID, 1, &fragmentData, NULL);
    glCompileShader(fragmentShaderID);
    checkInfoLog(fragmentShaderID, GL_COMPILE_STATUS);

    // Link shaders
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    checkInfoLog(programID, GL_LINK_STATUS);

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}
