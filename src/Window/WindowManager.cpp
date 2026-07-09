#include "WindowManager.h"
#include <GLFW/glfw3native.h>
#include <windows.h>
#include <iostream>

// 解决 Windows API 宏冲突
#ifdef CreateWindow
#undef CreateWindow
#endif

bool WindowManager::InitGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    return true;
}

GLFWwindow* WindowManager::CreateWindowN(int width, int height, const char* title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window\n";
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

void WindowManager::SetupTransparentWindow(GLFWwindow* window) {
    HWND hwnd = glfwGetWin32Window(window);
    SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}

void WindowManager::CenterWindow(GLFWwindow* window) {
    /*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);
    glfwSetWindowPos(window,
        monitorX + (mode->width - 900) / 2,
        monitorY + (mode->height - 650) / 2
    );*/
    glfwSetWindowPos(window, 0, 0);
}

void WindowManager::DestroyWindow(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
    }
}

void WindowManager::TerminateGLFW() {
    glfwTerminate();
}