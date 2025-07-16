#pragma once
#include "Window.h"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    Window* window;

    void update();
    void render();

};
