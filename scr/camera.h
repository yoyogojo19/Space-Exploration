#pragma once
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#ifndef CAMERA_H
#define CAMERA_H

struct camera {
    glm::vec3 pos;
    glm::vec2 dir;
    glm::vec3 front;
    glm::mat4 ProjectionMatrix;

    camera() : pos(0.0f,0.0f,-1.0f), dir(0.0f, 90.0f) {}
    camera(glm::vec3 pos, glm::vec2 dir) : pos(pos), dir(dir) {}

    glm::mat4 calculateMVP(const glm::mat4 &model) {
        glm::mat4 view = lookAt(pos, pos+front, glm::vec3(0.0f,1.0f,0.0f));
        return ProjectionMatrix*view*model;
    }

    void calculateProjectionMatrix(const float &fov, const float &ar, const float &nearz, const float &farz) {
        ProjectionMatrix = glm::perspective(fov, ar, nearz, farz);
    }

    void moveCamera(GLFWwindow *window, const double &DT) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pos.x += cos(glm::radians(dir.y)) * DT;
            pos.z += sin(glm::radians(dir.y)) * DT;
        } 
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pos.x -= cos(glm::radians(dir.y)) * DT;
            pos.z -= sin(glm::radians(dir.y)) * DT;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pos.x += -sin(glm::radians(dir.y)) * DT;
            pos.z += cos(glm::radians(dir.y)) * DT;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pos.x -= -sin(glm::radians(dir.y)) * DT;
            pos.z -= cos(glm::radians(dir.y)) * DT;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            pos.y += DT;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            pos.y -= DT;
        }
    }
};

#endif
inline bool firstMouse = true;
inline float lastX;
inline float lastY;
inline camera MainCam;

// code from https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp and slightly modified

inline void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse){
        glm::vec3 camfront;
        camfront.x = cos(glm::radians(MainCam.dir.y)) * cos(glm::radians(MainCam.dir.x));
        camfront.y = sin(glm::radians(MainCam.dir.x));
        camfront.z = sin(glm::radians(MainCam.dir.y)) * cos(glm::radians(MainCam.dir.x));
        MainCam.front = glm::normalize(camfront);
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.7f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    
        MainCam.dir.y += xoffset;
        MainCam.dir.x += yoffset;

        // make sure that when dir.x is out of bounds, screen doesn't get flipped
        if (MainCam.dir.x > 89.0f)
            MainCam.dir.x = 89.0f;
        if (MainCam.dir.x < -89.0f)
            MainCam.dir.x = -89.0f;

        glm::vec3 camfront;
        camfront.x = cos(glm::radians(MainCam.dir.y)) * cos(glm::radians(MainCam.dir.x));
        camfront.y = sin(glm::radians(MainCam.dir.x));
        camfront.z = sin(glm::radians(MainCam.dir.y)) * cos(glm::radians(MainCam.dir.x));
        MainCam.front = glm::normalize(camfront);
        }
} 