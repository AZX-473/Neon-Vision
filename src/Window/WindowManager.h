#pragma once
#include <GLFW/glfw3.h>

class WindowManager {
public:
    static bool InitGLFW();
    static GLFWwindow* CreateWindowN(int width, int height, const char* title);
    static void SetupTransparentWindow(GLFWwindow* window);
    static void CenterWindow(GLFWwindow* window);
    static void DestroyWindow(GLFWwindow* window);
    static void TerminateGLFW();
};