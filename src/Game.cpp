#include "Game.h"
#include <glad/glad.h>
#include <iostream>
#include <math.h>

Game::Game(int width, int height, const char* title)  {
    window = new Window(width, height, title);
    if (!window->isInitialized()) {
        std::cerr << "Failed to initialize window." << std::endl;
        exit(EXIT_FAILURE);
    }

    ourShader = Shader("../shaders/shader.vs", "../shaders/shader.fs");

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
    ourShader.setFloat("ourColor", greenValue);
}

void Game::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use(); // Use the shader program
    glBindVertexArray(VAO);          // Bind the VAO with triangle data
    glDrawArrays(GL_TRIANGLES, 0, 3); // Draw 3 vertices (1 triangle)

}