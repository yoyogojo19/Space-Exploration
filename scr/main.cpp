#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdint>

uint16_t pack(uint8_t x, uint8_t y, uint8_t z) {
    return (uint16_t)(x & 31u << 10) | (uint16_t)(y & 31u << 5) | (uint16_t)(z & 31u);
}

int main() {
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Minecraft Clone", monitor, NULL); 
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    std::cout << "\033[2J";
    if (glewInit() != GLEW_OK) {
        std::cerr << "glew failed to load\n";
        return -1;
    }
    
    MainCam.calculateProjectionMatrix(glm::radians(90.0f), 1920.0/1080.0, 0.05, 1000.0);

    float quadVert[] = {
        0.0f,0.0f,0.0f, 0.0f,1.0f,
        0.0f,1.0f,0.0f, 0.0f,0.0f,
        1.0f,1.0f,0.0f, 1.0f,0.0f,
        1.0f,0.0f,0.0f, 1.0f,1.0f
    };

    uint16_t verticies[] = {
        pack(0,0,0),
        pack(1,0,0),
        pack(0,1,0),
        pack(1,1,0),
        pack(0,0,1),
        pack(1,0,1),
        pack(0,1,1),
        pack(1,1,1)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), quadVert, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    shader Shaders("../shaders/triangle.vs.glsl", "../shaders/triangle.fs.glsl");
    textures Texture("../textures/rock.png");
    
    Shaders.use();
    Shaders.linkInt("ourTexture", 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    double deltaTime;
    double chrono = glfwGetTime();
    double avFPS = 0.0;
    unsigned int frames = 0;
    glfwSetCursorPosCallback(window, mouse_callback);
    
    while (!glfwWindowShouldClose(window)) {
        
        deltaTime = glfwGetTime() - chrono;
        chrono = glfwGetTime();
        avFPS = (avFPS*static_cast<double>(frames) + 1.0/deltaTime) / static_cast<double>(frames+1);
        frames += 1;
        // if (frames % 10 == 9) {
        //     std::cout << "Avrege FPS : " << 1.0/deltaTime << "\n\033[H";
        // } else {
        //     std::cout << "Avrege FPS : " << 1.0/deltaTime << "\n";
        // }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Shaders.linkMat4("mvp", MainCam.calculateMVP(glm::mat4(1.0f)));
        
        glBindVertexArray(VAO);
        Texture.bind(0);
        
        MainCam.moveCamera(window, deltaTime);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "\033[2J";
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(Shaders.ID);    
    glfwTerminate();

    return 0;
}