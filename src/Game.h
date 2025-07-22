#pragma once
#include "Window.h"

class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

private:
    Window* window;

    void update();
    void render();

};
