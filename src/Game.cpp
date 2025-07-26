#include "Game.h"
#include <glad/glad.h>
#include <iostream>

//TEMP STORAGE

//This is the source code for the vertex shader
const char*vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x,aPos.y, aPos.z, 1.0);\n"
"}\0";

//This is the source code for the fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

Game::Game(int width, int height, const char* title) {
    window = new Window(width, height, title);
    if (!window->isInitialized()) {
        std::cerr << "Failed to initialize window." << std::endl;
        exit(EXIT_FAILURE);
    }
}

Game::~Game() {
    delete window;
}

void Game::run() {
    while (!window->shouldClose()) {
        update();
        render();
        window->swapBuffers();
        window->pollEvents();
    }
}

void Game::update() {
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    }; 

    //Generate and bind a Vertex Buffer Object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Compile and link the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);


    
    
}

void Game::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}