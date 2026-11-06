#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "chunck.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sys/types.h>
#include <vector>


int main() {
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Space Exploration Game", monitor, NULL); 
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    std::cout << "\033[2J";
    if (glewInit() != GLEW_OK) {
        std::cerr << "glew failed to load\n";
        return -1;
    }
    
    MainCam.calculateProjectionMatrix(glm::radians(90.0f), 1920.0/1080.0, 0.05, 1000.0);

    float quadVert[] = {
        -0.5f,-0.5f,0.5f, 0.0f,1.0f,
        -0.5f,0.5f,0.5f, 0.0f,0.0f,
        0.5f,0.5f,0.5f, 1.0f,0.0f,
        0.5f,-0.5f,0.5f, 1.0f,1.0f
    };

    chunck MyChunck;

    for(int i = 0; i < MyChunck.size; i++){
        for (int k = 0; k < MyChunck.size; k++) {
            for (int j = 0; j < 2; j++){
                MyChunck.setBlock(i,j,k,blocks(ROCK));
            }
        }
    }
    
    std::vector<uint> positions = MyChunck.generateTerrin();
    

    unsigned int VBO, VAO, instanceVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), quadVert, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(uint), &positions.front(), GL_STATIC_DRAW);
    
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(uint), (void*)0);
    glEnableVertexAttribArray(2);
    
    glVertexAttribDivisor(2, 1);
    
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
        if (frames > 1) {
            std::cout << "Avrege FPS : " << 1.0/deltaTime << "\n\033[F";
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Shaders.linkMat4("mvp", MainCam.calculateMVP(glm::mat4(1.0f)));
        
        glBindVertexArray(VAO);
        Texture.bind(0);
        glDrawArraysInstanced(GL_QUADS, 0, 4, positions.size());
        glBindVertexArray(0);
        
        MainCam.moveCamera(window, deltaTime);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "Avrege FPS : " << 1.0/deltaTime << "\n";
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(Shaders.ID);    
    glfwTerminate();

    return 0;
}