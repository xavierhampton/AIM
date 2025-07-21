#pragma once
#include <GLFW/glfw3.h>

class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        bool isInitialized();
        bool shouldClose();
        void swapBuffers();
        void pollEvents();
        
    private:
        GLFWwindow* window;
        bool initialized;

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

};