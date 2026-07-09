#pragma once
#include <GLFW/glfw3.h>

class ImGuiManager {
public:
    static bool Init(GLFWwindow* window);
    static void NewFrame();
    static void Render();
    static void Shutdown();
    static void SetStyle();
};