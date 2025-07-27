#include "Game.h"
#include <glad/glad.h>
#include <iostream>
#include <math.h>

//TEMP STORAGE

//This is the source code for the vertex shader
const char*vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x,aPos.y, aPos.z, 1.0);\n"
"    ourColor = aColor;\n"
"}\0";

//This is the source code for the fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\0";

Game::Game(int width, int height, const char* title) {
    window = new Window(width, height, title);
    if (!window->isInitialized()) {
        std::cerr << "Failed to initialize window." << std::endl;
        exit(EXIT_FAILURE);
    }

       float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
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
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
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
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // Oscillate between 0 and 1
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUseProgram(shaderProgram); // Activate the shader program
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // Set the color
}

void Game::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);          // Bind the VAO with triangle data
    glDrawArrays(GL_TRIANGLES, 0, 3); // Draw 3 vertices (1 triangle)

}