#include "Game.h"
#include <glad/glad.h>
#include <iostream>

//TEMP STORAGE

//This is the source code for the vertex shader
const char*vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x,aPos.y, aPos.z, 1.0);\n"
"    vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

//This is the source code for the fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\0";

Game::Game(int width, int height, const char* title) {
    window = new Window(width, height, title);
    if (!window->isInitialized()) {
        std::cerr << "Failed to initialize window." << std::endl;
        exit(EXIT_FAILURE);
    }

       float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    }; 

    //Generate a Vertex Array Object (VAO) and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Generate a buffer and bind it to the GL_ARRAY_BUFFER target
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Compile and link the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    //Compile and link the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    //Create a shader program and link the shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Delete the shaders as they're no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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
    
}

void Game::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);     // Activate the shader
    glBindVertexArray(VAO);          // Bind the VAO with triangle data
    glDrawArrays(GL_TRIANGLES, 0, 3); // Draw 3 vertices (1 triangle)

}