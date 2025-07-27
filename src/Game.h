#pragma once
#include "Window.h"
#include "Shader.h"

class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

private:
    Window* window;
    
    unsigned int VBO;
    unsigned int VAO;
    Shader ourShader;

    void update();
    void render();

};
