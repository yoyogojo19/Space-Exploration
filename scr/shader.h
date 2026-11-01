#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <climits>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef SHADER_H
#define SHADER_H

// code from https://learnopengl.com/Getting-started/Shaders

struct shader {
    unsigned int ID;
    
    shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERREUR::FICHIER::N'A_PAS_PUS_ÊTRE_CHARGER\n";
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int success;
        char log[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, log);
            std::cout << "ERREUR::SHADER::VERTEX::COMPILATION_ÉCHOUER\n" << log << "\n";
        }
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, log);
            std::cout << "ERREUR::SHADER::FRAGMENT::COMPILATION_ÉCHOUER\n" << log << "\n";
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, log);
            std::cout << "ERREUR::SHADER::PROGRAME::LIAGE\n" << log << "\n";
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        glUseProgram(ID);
    }

    void linkMat4(const char* name, glm::mat4 matrix) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, value_ptr(matrix));
    }
    
    void linkMat3(const char* name, glm::mat3 matrix) {
        glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, value_ptr(matrix));
    }

    void linkMat2(const char* name, glm::mat2 matrix) {
        glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, value_ptr(matrix));
    }

    void linkVec4(const char* name, glm::vec4 vector) {
        glUniform4f(glGetUniformLocation(ID, name), vector.x, vector.y, vector.z, vector.w);
    }

    void linkVec3(const char* name, glm::vec3 vector) {
        glUniform3f(glGetUniformLocation(ID, name), vector.x, vector.y, vector.z);
    }

    void linkVec2(const char* name, glm::vec2 vector) {
        glUniform2f(glGetUniformLocation(ID, name), vector.x, vector.y);
    }

    void linkFloat(const char* name, float x) {
        glUniform1f(glGetUniformLocation(ID, name), x);
    }

    void linkInt(const char* name, int x) {
        glUniform1i(glGetUniformLocation(ID, name), x);
    }
};

#endif