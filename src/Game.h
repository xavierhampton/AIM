#pragma once
#include "Window.h"

class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

private:
    Window* window;
    
    unsigned int VBO;
    unsigned int VAO;
    unsigned int fragmentShader;
    unsigned int vertexShader;
    unsigned int shaderProgram;

    void update();
    void render();

};
