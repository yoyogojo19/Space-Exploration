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

int main() {
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Minecraft Clone", monitor, NULL); 
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    if (glewInit() != GLEW_OK) {
        std::cerr << "glew failed to load\n";
        return -1;
    }
    
    MainCam.calculateProjectionMatrix(glm::radians(90.0f), 1920.0/1080.0, 0.05, 1000.0);

    float verticies[] = {
        0.0f,0.0f,0.0f, 1.0f,1.0f,
        1.0f,0.0f,0.0f, 0.0f,1.0f,
        1.0f,1.0f,0.0f, 0.0f,0.0f,
        0.0f,1.0f,0.0f, 1.0f,0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    shader Shaders("../shaders/triangle.vs.glsl", "../shaders/triangle.fs.glsl");
    textures Texture("../textures/textureTest.png");
    
    Shaders.use();
    Shaders.linkInt("ourTexture", 0);
    glEnable(GL_DEPTH_TEST);

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
        std::cout << "Avrege FPS : " << 1.0/deltaTime << "\n";

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Shaders.linkMat4("mvp", MainCam.calculateMVP(glm::mat4(1.0f)));
        
        glBindVertexArray(VAO);
        Texture.bind(0);
        glDrawArrays(GL_QUADS, 0, 4);

        MainCam.moveCamera(window, deltaTime);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(Shaders.ID);    
    glfwTerminate();

    return 0;
}